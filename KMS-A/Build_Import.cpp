
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Import.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Version.h>

#include <KMS/Build/Import.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Import.cfg")

#define DEFAULT_EXPORT_FOLDER ("K:\\Export")

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

                lI.InitConfigurator(&lC);

                Dbg::gLog.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lResult = lI.Run();
            }
            KMS_CATCH_RESULT(lResult);

            return lResult;
        }

        Import::Import() : mImport("Import")
        {
            mRepositories.push_back(File::Folder(DEFAULT_EXPORT_FOLDER));
        }

        Import::~Import() {}

        void Import::AddDependency       (const char* aD) { mDependencies     .insert(aD); }
        void Import::AddOSIndependentDeps(const char* aD) { mOSIndependentDeps.insert(aD); }
        void Import::AddRepository       (const char* aR) { mRepositories.push_back(File::Folder(aR)); }

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

            for (File::Folder& lR : mRepositories)
            {
                if (lR.DoesFolderExist(lProduct))
                {
                    File::Folder lProductFolder(lR, lProduct);

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

            for (std::string lD : mDependencies)
            {
                ImportDependency(lD.c_str(), false);
            }

            for (std::string lD : mOSIndependentDeps)
            {
                ImportDependency(lD.c_str(), true);
            }

            return 0;
        }

        // ===== Cfg::Configurable ==========================================

        bool Import::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            CFG_CALL("Dependencies"     , AddDependency       );
            CFG_CALL("OSIndependentDeps", AddOSIndependentDeps);
            CFG_CALL("Repositories"     , AddRepository       );

            #ifdef _KMS_DARWIN_
                CFG_CALL("DarwinDependencies", AddDependency);
                CFG_CALL("DarwinRepositories", AddRepository);
            #endif

            #ifdef _KMS_LINUX_
                CFG_CALL("LinuxDependencies", AddDependency);
                CFG_CALL("LinuxRepositories", AddRepository);
            #endif

            #ifdef _KMS_WINDOWS_
                CFG_CALL("WindowsDependencies", AddDependency);
                CFG_CALL("WindowsRepositories", AddRepository);
            #endif

            return Configurable::AddAttribute(aA, aV);
        }

        void Import::DisplayHelp(FILE* aOut) const
        {
            fprintf(aOut,
                "===== KMS::Build::Import =====\n"
                "Dependencies += {Product};{Version}\n"
                "    Add a dependency\n"
                "OSIndependentDeps += {Product};{Version}\n"
                "    Add an OS independent dependency\n"
                "Repositories += {Path}\n"
                "    Add a repository\n");

            #ifdef _KMS_DARWIN_
                fprintf(aOut,
                    "DarawinDependencies += {Product};{Version}\n"
                    "    See Dependencies\n"
                    "DarwinRepositories += {Path}\n"
                    "    See repositories\n");
            #endif

            #ifdef _KMS_LINUX_
                fprintf(aOut,
                    "LinuxDependencies += {Product};{Version}\n"
                    "    See Dependencies\n"
                    "LinuxRepositories += {Path}\n"
                    "    See repositories\n");
            #endif

            #ifdef _KMS_WINDOWS_
                fprintf(aOut,
                    "WindowsDependencies += {Product};{Version}\n"
                    "    See Dependencies\n"
                    "WindowsRepositories += {Path}\n"
                    "    See repositories\n");
            #endif

            Configurable::DisplayHelp(aOut);
        }

    }
}
