
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Import.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Build/Package.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/HTTP/Client.h>
#include <KMS/Main.h>
#include <KMS/Net/Socket.h>
#include <KMS/Version.h>

#include <KMS/Build/Import.h>

KMS_RESULT_STATIC(RESULT_DEPENDENCY_NOT_FOUND);

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Import.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_DEPENDENCIES       ("Dependencies += {Product};{Version}");
static const KMS::Cfg::MetaData MD_OS_INDEPENDENT_DEPS("OSIndependentDeps += {Product};{Version}");
static const KMS::Cfg::MetaData MD_REPOSITORIES       ("Repositories += {Path}");
static const KMS::Cfg::MetaData MD_SERVERS            ("Servers += {URL}");

#ifdef _KMS_DARWIN_
    #define NAME_OS "Darwin"
    #define NO_OS_0 "Linux"
    #define NO_OS_1 "Windows"
#endif

#ifdef _KMS_LINUX_
    #define NAME_OS "Linux"
    #define NO_OS_0 "Darwin"
    #define NO_OS_1 "Windows"
#endif

#ifdef _KMS_WINDOWS_
    #define NAME_OS "Windows"
    #define NO_OS_0 "Darwin"
    #define NO_OS_1 "Linux"
#endif

static const KMS::Cfg::MetaData MD_OS_DEPENDENCIES(NAME_OS "Dependencies += {Product};{Version}");

static const char* SILENCE[] =
{
    NO_OS_0 "Dependencies", NO_OS_1 "Dependencies",

    nullptr
};

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Import::SERVER_DEFAULT = "https://github.com/martindubois/";

        int Import::Main(int aCount, const char** aVector)
        {
            Net::Thread_Startup();

            KMS_MAIN_BEGIN;
            {
                Build::Import lI;

                lConfigurator.SetSilence(SILENCE);

                lConfigurator.AddConfigurable(&lI);

                lConfigurator.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lConfigurator.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);

                KMS_MAIN_PARSE_ARGS(aCount, aVector);

                KMS_MAIN_VALIDATE;

                lResult = lI.Run();
            }
            KMS_MAIN_END;

            Net::Thread_Cleanup();

            KMS_MAIN_RETURN;
        }

        Import::Import() : mImport("Import"), mTmpFolder(File::Folder::Id::TEMPORARY)
        {
            mDependencies     .SetCreator(DI::String::Create);
            mOSIndependentDeps.SetCreator(DI::String::Create);
            mRepositories     .SetCreator(DI::Folder::Create);
            mServers          .SetCreator(DI::String_Expand::Create);

            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mDependencies     , false); AddEntry("Dependencies"     , lEntry, &MD_DEPENDENCIES);
            lEntry.Set(&mOSIndependentDeps, false); AddEntry("OSIndependentDeps", lEntry, &MD_OS_INDEPENDENT_DEPS);
            lEntry.Set(&mRepositories     , false); AddEntry("Repositories"     , lEntry, &MD_REPOSITORIES);
            lEntry.Set(&mServers          , false); AddEntry("Servers"          , lEntry, &MD_SERVERS);

            lEntry.Set(&mDependencies, false);  AddEntry(NAME_OS "Dependencies", lEntry, &MD_OS_DEPENDENCIES);

            lEntry.Set(new DI::Folder(REPOSITORY_DEFAULT), true);  mRepositories.AddEntry(lEntry);

            lEntry.Set(new DI::String_Expand(SERVER_DEFAULT), true); mServers.AddEntry(lEntry);
        }

        void Import::ImportDependency(const char* aDependency, bool aOSIndependent)
        {
            Package lPackage(aOSIndependent);

            lPackage.Parse(aDependency);

            char lFileName[FILE_LENGTH];

            lPackage.GetFileName(lFileName, sizeof(lFileName));

            for (const auto& lEntry : mRepositories.mInternal)
            {
                assert(nullptr != lEntry);

                auto lR = dynamic_cast<const DI::Folder*>(lEntry.Get());

                if (ImportDependency_Folder(lPackage, lR, lFileName))
                {
                    return;
                }
            }

            for (const auto& lEntry : mServers.mInternal)
            {
                assert(nullptr != lEntry);

                auto lS = dynamic_cast<const DI::String_Expand*>(lEntry.Get());

                if (ImportDependency_Server(lPackage, lS->Get(), lFileName))
                {
                    return;
                }
            }

            KMS_EXCEPTION(RESULT_DEPENDENCY_NOT_FOUND, "Dependency not found", aDependency);
        }

        int Import::Run()
        {
            // TODO Make the Import folder empty

            if (!mImport.DoesExist())
            {
                mImport.Create();
            }

            auto lIT = new Dbg::Stats_Timer("ImportTime");

            for (const auto& lEntry : mDependencies.mInternal)
            {
                assert(nullptr != lEntry);

                lIT->Start();

                auto lD = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lD);

                ImportDependency(*lD, false);

                lIT->Stop();
            }

            for (const auto& lEntry : mOSIndependentDeps.mInternal)
            {
                assert(nullptr != lEntry);

                lIT->Start();

                auto lD = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lD);

                ImportDependency(*lD, true);

                lIT->Stop();
            }

            return 0;
        }

        // ===== DI::Container ==============================================

        void Import::Validate() const
        {
            DI::Dictionary::Validate();

            for (const auto& lEntry : mDependencies.mInternal)
            {
                auto lS = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lS);

                KMS_EXCEPTION_ASSERT(!lS->GetString().empty(), RESULT_INVALID_CONFIG, "Empty dependency", "");
            }

            for (const auto& lEntry : mOSIndependentDeps.mInternal)
            {
                auto lS = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lS);

                KMS_EXCEPTION_ASSERT(!lS->GetString().empty(), RESULT_INVALID_CONFIG, "Empty dependency", "");
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Import::ImportDependency_Folder(const Package& aPackage, const DI::Folder* aR, const char* aFileName)
        {
            assert(nullptr != aR);

            auto lResult = false;

            auto lProductName = aPackage.GetProductName();

            KMS_DBG_LOG_INFO();
            Dbg::gLog.WriteMessage(aR->GetFolder().GetPath());
            Dbg::gLog.WriteMessage(lProductName);
            Dbg::gLog.WriteMessage(aFileName);

            File::Folder lProductFolder(*aR, lProductName);
            if (lProductFolder.DoesExist())
            {
                if (lProductFolder.DoesFileExist(aFileName))
                {
                    std::cout << "Importing " << lProductName << " " << aPackage.GetVersion() << " from " << *aR << std::endl;

                    mImport.Uncompress(lProductFolder, aFileName, File::Folder::FLAG_OVERWRITE);

                    lResult = true;
                }
            }

            return lResult;
        }

        bool Import::ImportDependency_Server(const Package& aPackage, const char* aServer, const char* aFileName)
        {
            assert(nullptr != aServer);
            assert(nullptr != aFileName);

            auto lSecure = strcmp("https://", aServer);

            HTTP::Client lClient(lSecure);

            char lTag[LINE_LENGTH];
            char lURL[LINE_LENGTH * 2];

            aPackage.GetTagName(lTag, sizeof(lTag));

            sprintf_s(lURL, "%s%s/releases/download/%s/%s", aServer, aPackage.GetProductName(), lTag, aFileName);

            KMS_DBG_LOG_INFO();
            Dbg::gLog.WriteMessage(lURL);

            auto lResult = false;

            File::Binary lOutFile(mTmpFolder, aFileName);

            try
            {
                std::cout << "Trying to import " << aPackage.GetProductName() << " " << aPackage.GetVersion() << " from " << aServer << std::endl;

                lClient.Get(lURL, &lOutFile);

                mImport.Uncompress(mTmpFolder, aFileName, File::Folder::FLAG_OVERWRITE);

                lResult = true;
            }
            KMS_CATCH;

            return lResult;
        }

    }
}
