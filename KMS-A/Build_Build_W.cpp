
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

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

        void Build::AddProcessor(const char* aP) { assert(NULL != aP); mWindowsProcessors.AddEntry(new DI::String(aP), true); }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile_VisualStudio(const char* aC)
        {
            for (const DI::Container::Entry& lEntry : mWindowsProcessors.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lP);

                File::Folder lProgramFiles(File::Folder::Id::PROGRAM_FILES);
                File::Folder lBin(lProgramFiles, MSBUILD_FOLDER);

                Proc::Process lProcess(lBin, "MSBuild.exe");

                lProcess.AddArgument("Solution.sln");
                lProcess.AddArgument("/target:rebuild");

                lProcess.AddArgument((std::string("/Property:Configuration=") + aC).c_str());
                lProcess.AddArgument(("/property:Platform=" + std::string(lP->Get())).c_str());

                lProcess.Run(1000 * 60 * 5);

                if (0 != lProcess.GetExitCode())
                {
                    KMS_EXCEPTION(BUILD_COMPILE_FAILED, "The compilation failed", lProcess.GetCmdLine());
                }
            }
        }

        void Build::Package_Components(const char* aC)
        {
            File::Folder lBinaries(mTempFolder, "Binaries");
            File::Folder lLibraries(mTempFolder, "Libraries");

            for (const DI::Container::Entry& lEntry : mWindowsProcessors.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lP);

                std::string lCfg = aC;

                lCfg += "_";
                lCfg += lP->Get();

                File::Folder lBin(lBinaries, lCfg.c_str());
                File::Folder lLib(lLibraries, lCfg.c_str());

                std::string lOutDir = (*lP == "x86") ? aC : std::string(*lP) + "\\" + aC;

                File::Folder lOut_Src(lOutDir.c_str());

                lBin.Create();
                lLib.Create();

                for (const DI::Container::Entry& lEntry : mBinaries.mInternal)
                {
                    assert(NULL != lEntry);

                    const DI::String* lB = dynamic_cast<const DI::String*>(lEntry.Get());
                    assert(NULL != lB);

                    lOut_Src.Copy(lBin, (std::string(*lB) + ".exe").c_str());
                    lOut_Src.Copy(lBin, (std::string(*lB) + ".pdb").c_str());
                }

                for (const DI::Container::Entry& lEntry : mLibraries.mInternal)
                {
                    assert(NULL != lEntry);

                    const DI::String* lL = dynamic_cast<const DI::String*>(lEntry.Get());
                    assert(NULL != lL);

                    lOut_Src.Copy(lLib, (std::string(*lL) + ".lib").c_str());
                    lOut_Src.Copy(lLib, (std::string(*lL) + ".pdb").c_str());
                }
            }
        }

        void Build::Test(const char* aC)
        {
            for (const DI::Container::Entry& lEntry : mWindowsProcessors.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lP);

                for (const DI::Container::Entry& lEntry : mTests.mInternal)
                {
                    assert(NULL != lEntry);

                    const DI::String* lT = dynamic_cast<const DI::String*>(lEntry.Get());
                    assert(NULL != lT);

                    std::string lOutDir = (*lP == "x86") ? aC : std::string(*lP) + "\\" + aC;

                    Proc::Process lProcess(lOutDir.c_str(), (std::string(*lT) + ".exe").c_str());

                    lProcess.AddArgument("Groups+=Auto");

                    lProcess.Run(1000 * 60 * 5);

                    if (0 != lProcess.GetExitCode())
                    {
                        KMS_EXCEPTION(BUILD_TEST_FAILED, "The test failed", lProcess.GetCmdLine());
                    }
                }
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Export_WindowsFile_MSI()
        {
            if (0 < mWindowsFile_MSI.GetLength())
            {
                char lDstName[PATH_LENGTH];

                sprintf_s(lDstName, "%s_%u.%u.%u.msi", mProduct.Get(), mVersion.GetMajor(), mVersion.GetMinor(), mVersion.GetBuild());

                File::Folder::CURRENT.Copy(mProductFolder, mWindowsFile_MSI.Get(), lDstName);
            }
        }

    }
}
