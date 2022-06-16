
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Process.h>
#include <KMS/Build/Make.h>
#include <KMS/Config/Configurator.h>

#include <KMS/Build/Build.h>

namespace KMS
{
    namespace Build
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile(const char* aC)
        {
            Config::Configurator lC;
            Make lM;

            lM.InitConfigurator(&lC);

            lC.Init();
            lC.ParseFile(File::Folder(File::Folder::Id::CURRENT), "KMS-Build.cfg");

            lM.SetConfiguration(aC);

            lM.AddOperation("Clean");
            lM.AddOperation("Make");

            int lRet = lM.Run();
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(BUILD_COMPILE, "KMS::Build::Make::Run failed", lRet);
            }
        }

        void Build::Package_Component(const char* aC)
        {
            File::Folder lBinaries(mTempFolder , "Binaries" );
            File::Folder lLibraries(mTempFolder, "Libraries");

            File::Folder lBin(lBinaries , aC);
            File::Folder lLib(lLibraries, aC);

            File::Folder lBin_Src((std::string("Binaries/" ) + aC).c_str());
            File::Folder lLib_Src((std::string("Libraries/") + aC).c_str());

            lBin.Create();
            lLib.Create();

            for (std::string lB : mBinaries)
            {
                lBin_Src.Copy(lBin, lB.c_str());
            }

            for (std::string lB : mLibraries)
            {
                lLib_Src.Copy(lLib, (lB + ".a").c_str());
            }
        }

        void Build::Test(const char* aC)
        {
            for (std::string lT : mTests)
            {
                KMS::Process lProcess((std::string("Binaries/") + aC).c_str(), lT.c_str());

                lProcess.AddArgument("Group=Auto");

                if (0 != lProcess.Run())
                {
                    KMS_EXCEPTION_WITH_INFO(BUILD_TEST, "The test failed", lProcess.GetCmdLine());
                }
            }
        }
    }
}

