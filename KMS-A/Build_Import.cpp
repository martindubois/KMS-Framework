
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Import.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Config/Configurator.h>

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
                KMS::Build::Import        lI;
                KMS::Config::Configurator lC;

                lI.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE, false);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT), CONFIG_FILE);
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

        void Import::AddDependency(const char* aD) { mDependencies.insert(aD); }
        void Import::AddRepository(const char* aR) { mRepositories.push_back(File::Folder(aR)); }

        void Import::ImportDependency(const char* aDependency)
        {
            char lProduct[1024];

            strcpy_s(lProduct, aDependency);

            char* lPtr = strrchr(lProduct, '_');
            if (NULL == lPtr)
            {
                KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid dependency", aDependency);
            }

            *lPtr = '\0';

            for (File::Folder& lR : mRepositories)
            {
                if (lR.DoesFolderExist(lProduct))
                {
                    File::Folder lProductFolder(lR, lProduct);

                    char lDependency[1024];

                    sprintf_s(lDependency, "%s.zip", aDependency);

                    if (lProductFolder.DoesFileExist(lDependency))
                    {
                        mImport.Uncompress(lProductFolder, lDependency);
                        return;
                    }
                }
            }

            KMS_EXCEPTION_WITH_INFO(DEPENDENCY, "Dependency not found", aDependency);
        }

        int Import::Run()
        {
            if (mImport.DoesExist())
            {
                mImport.Delete();
            }

            mImport.Create();

            for (std::string lD : mDependencies)
            {
                ImportDependency(lD.c_str());
            }

            return 0;
        }

        // ===== Config::Configurable =======================================

        bool Import::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            CFG_CALL("Dependencies", AddDependency);
            CFG_CALL("Repositories", AddRepository);

            return Configurable::AddAttribute(aA, aV);
        }

    }
}
