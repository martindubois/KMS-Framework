
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Import.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Main.h>
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

        int Import::Main(int aCount, const char** aVector)
        {
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

            KMS_MAIN_RETURN;
        }

        Import::Import() : mImport("Import")
        {
            mDependencies     .SetCreator(DI::String::Create);
            mOSIndependentDeps.SetCreator(DI::String::Create);
            mRepositories     .SetCreator(DI::Folder::Create);

            AddEntry("Dependencies"     , &mDependencies     , false, &MD_DEPENDENCIES);
            AddEntry("OSIndependentDeps", &mOSIndependentDeps, false, &MD_OS_INDEPENDENT_DEPS);
            AddEntry("Repositories"     , &mRepositories     , false, &MD_REPOSITORIES);

            AddEntry(NAME_OS "Dependencies", &mDependencies, false, &MD_OS_DEPENDENCIES);

            #ifdef _KMS_LINUX_
                File::Folder lExport(File::Folder::Id::HOME, "Export");
            #endif

            #ifdef _KMS_WINDOWS_
                File::Folder lExport("K:\\Export");
            #endif

            mRepositories.AddEntry(new DI::Folder(lExport), true);
        }

        Import::~Import() {}

        void Import::AddDependency       (const char* aD) { mDependencies     .AddEntry(new DI::String(aD), true); }
        void Import::AddOSIndependentDeps(const char* aD) { mOSIndependentDeps.AddEntry(new DI::String(aD), true); }
        void Import::AddRepository       (const char* aR) { mRepositories     .AddEntry(new DI::Folder(aR), true); }

        void Import::ImportDependency(const char* aDependency, bool aOSIndependent)
        {
            char lProduct[NAME_LENGTH];
            char lVersion[NAME_LENGTH];

            if (2 != sscanf_s(aDependency, "%[^;] ; %[^\n\r]", lProduct SizeInfo(lProduct), lVersion SizeInfo(lVersion)))
            {
                KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid dependency", aDependency);
            }

            Version lV(lVersion);

            unsigned int lFlags = aOSIndependent ? Version::FLAG_OS_INDEPENDENT : 0;

            char lPackage[FILE_LENGTH];

            lV.GetPackageName(lProduct, lPackage, sizeof(lPackage), lFlags);

            for (const auto& lEntry : mRepositories.mInternal)
            {
                assert(nullptr != lEntry);

                auto lR = dynamic_cast<const DI::Folder*>(lEntry.Get());
                assert(nullptr != lR);

                if (lR->GetFolder().DoesFolderExist(lProduct))
                {
                    File::Folder lProductFolder(*lR, lProduct);

                    if (lProductFolder.DoesFileExist(lPackage))
                    {
                        std::cout << "Importing " << lProduct << " " << lVersion << " from " << *lR << std::endl;

                        mImport.Uncompress(lProductFolder, lPackage);
                        return;
                    }
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

    }
}
