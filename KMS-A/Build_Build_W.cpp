
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Build/Build.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define MSBUILD_FOLDER ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Build::AddProcessor(const char* aP) { assert(NULL != aP); mProcessors.insert(aP); }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile(const char* aC)
        {
            for (std::string lP : mProcessors)
            {
                File::Folder lProgramFiles(File::Folder::Id::PROGRAM_FILES);
                File::Folder lBin(lProgramFiles, MSBUILD_FOLDER);

                KMS::Process lProcess(lBin, "MSBuild.exe");

                lProcess.AddArgument("Solution.sln");
                lProcess.AddArgument("/target:rebuild");

                lProcess.AddArgument(("/Property:Configuration=" + lC).c_str());
                lProcess.AddArgument(("/property:Platform="      + lP).c_str());

                if (0 != lProcess.Run())
                {
                    KMS_EXCEPTION_WITH_INFO(BUILD_COMPILE, "The compilation failed", lProcess.GetCmdLine());
                }
            }
        }

        void Build::Package_Component(const char* aC)
        {
            File::Folder lBinaries(mTempFolder, "Binaries");
            File::Folder lLibraries(mTempFolder, "Libraries");

            for (std::string lP : mProcessors)
            {
                std::string lCfg = aC;

                lCfg += "_";
                lCfg += lP;

                File::Folder lBin(lBinaries, lCfg.c_str());
                File::Folder lLib(lLibraries, lCfg.c_str());
                File::Folder lOut_Src((lP + "\\" + lC).c_str());

                lBin.Create();
                lLib.Create();

                for (std::string lB : mBinaries)
                {
                    lOut_Src.Copy(lBin, (lB + ".exe").c_str());
                    lOut_Src.Copy(lBin, (lB + ".pdb").c_str());
                }

                for (std::string lB : mLibraries)
                {
                    lOut_Src.Copy(lLib, (lB + ".lib").c_str());
                    lOut_Src.Copy(lLib, (lB + ".pdb").c_str());
                }
            }
        }

        void Build::Test(const char* aC)
        {
            for (std::string lP : mProcessors)
            {
                for (std::string lT : mTests)
                {
                    KMS::Process lProcess((lP + "\\" + aC).c_str(), (lT + ".exe").c_str());

                    lProcess.AddArgument("Group=Auto");

                    if (0 != lProcess.Run())
                    {
                        KMS_EXCEPTION_WITH_INFO(BUILD_TEST, "The test failed", lProcess.GetCmdLine());
                    }
                }
            }
        }

    }
}
