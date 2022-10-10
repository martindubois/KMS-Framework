
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Import.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Installer.h>
#include <KMS/Version.h>

#include <KMS/Build/Import.h>

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

    static const KMS::Cfg::MetaData MD_OS_DEPENDENCIES("DarwinDependencies += {Product};{Version}");
#endif

#ifdef _KMS_LINUX_
    #define NAME_OS "Linux"

    static const KMS::Cfg::MetaData MD_OS_DEPENDENCIES("LinuxDependencies += {Product};{Version}");
#endif

#ifdef _KMS_WINDOWS_
    #define NAME_OS "Windows"

    static const KMS::Cfg::MetaData MD_OS_DEPENDENCIES("WindowsDependencies += {Product};{Version}");
#endif

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        int Import::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);
            assert(NULL != aVector[0]);

            int lResult = __LINE__;

            try
            {
                KMS::Build::Import     lI;
                KMS::Cfg::Configurator lC;
                KMS::Installer         lInstaller;

                lC.AddConfigurable(&lI);
                lC.AddConfigurable(&lInstaller);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                Dbg::gLog.CloseLogFiles();

                lInstaller.Run();

                lResult = lI.Run();
            }
            KMS_CATCH_RESULT(lResult);

            return lResult;
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

            mRepositories.AddEntry(new DI::Folder(lExport));
        }

        Import::~Import() {}

        void Import::AddDependency       (const char* aD) { mDependencies     .AddEntry(new DI::String(aD)); }
        void Import::AddOSIndependentDeps(const char* aD) { mOSIndependentDeps.AddEntry(new DI::String(aD)); }
        void Import::AddRepository       (const char* aR) { mRepositories     .AddEntry(new DI::Folder(aR)); }

        void Import::ImportDependency(const char* aDependency, bool aOSIndependent)
        {
            char lProduct[NAME_LENGTH];
            char lVersion[NAME_LENGTH];

            if (2 != sscanf_s(aDependency, "%[^;] ; %[^\n\r]", lProduct SizeInfo(lProduct), lVersion SizeInfo(lVersion)))
            {
                KMS_EXCEPTION(CONFIG_FORMAT, "Invalid dependency", aDependency);
            }

            Version lV(lVersion);

            unsigned int lFlags = aOSIndependent ? Version::FLAG_OS_INDEPENDENT : 0;

            char lPackage[FILE_LENGTH];

            lV.GetPackageName(lProduct, lPackage, sizeof(lPackage), lFlags);

            const DI::Array::Internal& lInternal = mRepositories.GetInternal();
            for (const DI::Container::Entry& lEntry : lInternal)
            {
                assert(NULL != lEntry);

                const DI::Folder* lR = dynamic_cast<const DI::Folder*>(lEntry.Get());
                assert(NULL != lR);

                if (lR->Get().DoesFolderExist(lProduct))
                {
                    File::Folder lProductFolder(*lR, lProduct);

                    if (lProductFolder.DoesFileExist(lPackage))
                    {
                        mImport.Uncompress(lProductFolder, lPackage);
                        return;
                    }
                }
            }

            KMS_EXCEPTION(DEPENDENCY, "Dependency not found", aDependency);
        }

        int Import::Run()
        {
            // TODO Make the Import folder empty

            if (!mImport.DoesExist())
            {
                mImport.Create();
            }

            const DI::Array::Internal& lInternalD = mDependencies.GetInternal();
            for (const DI::Container::Entry& lEntry : lInternalD)
            {
                assert(NULL != lEntry);

                const DI::String* lD = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lD);

                ImportDependency(*lD, false);
            }

            const DI::Array::Internal& lInternalO = mOSIndependentDeps.GetInternal();
            for (const DI::Container::Entry& lEntry : lInternalO)
            {
                assert(NULL != lEntry);

                const DI::String* lD = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lD);

                ImportDependency(*lD, true);
            }

            return 0;
        }

    }
}
