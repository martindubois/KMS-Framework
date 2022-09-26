
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Import.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/DI/MetaData.h>
#include <KMS/Version.h>

#include <KMS/Build/Import.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Import.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI::MetaData MD_DEPENDENCIES       ("Dependencies"     , "Dependencies += {Product};{Version}");
static const KMS::DI::MetaData MD_OS_INDEPENDENT_DEPS("OSIndependentDeps", "OSIndependentDeps += {Product};{Version}");
static const KMS::DI::MetaData MD_REPOSITORIES       ("Repositories"     , "Repositories += {Path}");

#ifdef _KMS_DARWIN_
    static const KMS::DI::MetaData MD_OS_DEPENDENCIES("DarwinDependencies", "DarwinDependencies += {Product};{Version}");
#endif

#ifdef _KMS_LINUX_
    static const KMS::DI::MetaData MD_OS_DEPENDENCIES("LinuxDependencies", "LinuxDependencies += {Product};{Version}");
#endif

#ifdef _KMS_WINDOWS_
    static const KMS::DI::MetaData MD_OS_DEPENDENCIES("WindowsDependencies", "WindowsDependencies += {Product};{Version}");
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

                lC.AddConfigurable(&lI);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                Dbg::gLog.CloseLogFiles();

                lResult = lI.Run();
            }
            KMS_CATCH_RESULT(lResult);

            return lResult;
        }

        Import::Import()
            : DI::Dictionary(NULL)
            , mDependencies     (&MD_DEPENDENCIES)
            , mOSIndependentDeps(&MD_OS_INDEPENDENT_DEPS)
            , mRepositories     (&MD_REPOSITORIES)
            , mOSDependencies(&mDependencies, &MD_OS_DEPENDENCIES)
            , mImport("Import")
        {
            mDependencies     .SetCreator(DI::String::Create);
            mOSIndependentDeps.SetCreator(DI::String::Create);
            mRepositories     .SetCreator(DI::Folder::Create);

            AddEntry(&mDependencies);
            AddEntry(&mOSIndependentDeps);
            AddEntry(&mRepositories);

            AddEntry(&mOSDependencies);

            #ifdef _KMS_LINUX_
                File::Folder lExport(File::Folder::Id::HOME, "Export");
            #endif

            #ifdef _KMS_WINDOWS_
                File::Folder lExport("K:\\Export");
            #endif

            mRepositories.AddEntry(new DI::Folder(lExport, &DI::META_DATA_DELETE_OBJECT));
        }

        Import::~Import() {}

        void Import::AddDependency       (const char* aD) { mDependencies     .AddEntry(new DI::String(aD, &DI::META_DATA_DELETE_OBJECT)); }
        void Import::AddOSIndependentDeps(const char* aD) { mOSIndependentDeps.AddEntry(new DI::String(aD, &DI::META_DATA_DELETE_OBJECT)); }
        void Import::AddRepository       (const char* aR) { mRepositories     .AddEntry(new DI::Folder(aR, &DI::META_DATA_DELETE_OBJECT)); }

        void Import::ImportDependency(const char* aDependency, bool aOSIndependent)
        {
            char lProduct[NAME_LENGTH];
            char lVersion[NAME_LENGTH];

            if (2 != sscanf_s(aDependency, "%[^;] ; %[^\n\r]", lProduct SizeInfo(lProduct), lVersion SizeInfo(lVersion)))
            {
                KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid dependency", aDependency);
            }

            Version lV(lVersion);

            unsigned int lFlags = aOSIndependent ? Version::FLAG_OS_INDEPENDENT : 0;

            char lPackage[FILE_LENGTH];

            lV.GetPackageName(lProduct, lPackage, sizeof(lPackage), lFlags);

            const DI::Array::Internal& lInternal = mRepositories.GetInternal();
            for (DI::Object* lObj : lInternal)
            {
                assert(NULL != lObj);

                const DI::Folder* lR = dynamic_cast<const DI::Folder*>(lObj);
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

            KMS_EXCEPTION_WITH_INFO(DEPENDENCY, "Dependency not found", aDependency);
        }

        int Import::Run()
        {
            // TODO Make the Import folder empty

            if (!mImport.DoesExist())
            {
                mImport.Create();
            }

            const DI::Array::Internal& lInternalD = mDependencies.GetInternal();
            for (DI::Object* lObj : lInternalD)
            {
                assert(NULL != lObj);

                const DI::String* lD = dynamic_cast<const DI::String*>(lObj);
                assert(NULL != lD);

                ImportDependency(*lD, false);
            }

            const DI::Array::Internal& lInternalO = mOSIndependentDeps.GetInternal();
            for (DI::Object* lObj : lInternalO)
            {
                assert(NULL != lObj);

                const DI::String* lD = dynamic_cast<const DI::String*>(lObj);
                assert(NULL != lD);

                ImportDependency(*lD, true);
            }

            return 0;
        }

    }
}
