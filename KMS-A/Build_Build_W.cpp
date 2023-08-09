
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>
#include <KMS/Dbg/Stats_Timer.h>

#include <KMS/Build/Build.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define MSBUILD_FOLDER ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")

#define MSBUILD_ALLOWER_TIME_ms (1000 * 60 * 10) // 10 minutes
#define TEST_ALLOWED_TIME_ms    (1000 * 60 *  5) //  5 minutes

namespace KMS
{
    namespace Build
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile_VisualStudio(const char* aC)
        {
            auto lCT = new Dbg::Stats_Timer("CompileTime.Processor");

            for (const DI::Container::Entry& lEntry : mProcessors.mInternal)
            {
                lCT->Start();

                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                Compile_VisualStudio(aC, *lP);

                lCT->Stop();
            }
        }

        void Build::Compile_VisualStudio(const char* aC, const char* aP)
        {
            File::Folder lProgramFiles(File::Folder::Id::PROGRAM_FILES);
            File::Folder lBin(lProgramFiles, MSBUILD_FOLDER);

            Proc::Process lProcess(lBin, "MSBuild.exe");

            lProcess.AddArgument("Solution.sln");
            lProcess.AddArgument("/target:rebuild");

            lProcess.AddArgument((std::string("/Property:Configuration=") + aC).c_str());
            lProcess.AddArgument(("/property:Platform=" + std::string(aP)).c_str());

            lProcess.Run(MSBUILD_ALLOWER_TIME_ms);

            auto lRet = lProcess.GetExitCode();
            KMS_EXCEPTION_ASSERT(0 == lRet, BUILD_COMPILE_FAILED, "The compilation failed", lProcess.GetCmdLine());
        }

        void Build::Package_Components(const char* aC)
        {
            for (const auto& lEntry : mProcessors.mInternal)
            {
                assert(nullptr != lEntry);

                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                std::string lCfg = aC;

                lCfg += "_";
                lCfg += lP->Get();

                std::string lOutDir = (*lP == "x86") ? aC : std::string(*lP) + "\\" + aC;

                File::Folder lOut_Src(lOutDir.c_str());

                if (!mBinaries.IsEmpty())
                {
                    File::Folder lBin(mTmp_Binaries, lCfg.c_str());

                    lBin.Create();

                    for (const auto& lEntry : mBinaries.mInternal)
                    {
                        assert(nullptr != lEntry);

                        auto lB = dynamic_cast<const DI::String*>(lEntry.Get());
                        assert(nullptr != lB);

                        lOut_Src.Copy(lBin, (std::string(*lB) + ".exe").c_str());
                        lOut_Src.Copy(lBin, (std::string(*lB) + ".pdb").c_str());
                    }
                }

                if (!mLibraries.IsEmpty())
                {
                    File::Folder lLib(mTmp_Libraries, lCfg.c_str());

                    lLib.Create();

                    for (const auto& lEntry : mLibraries.mInternal)
                    {
                        assert(nullptr != lEntry);

                        auto lL = dynamic_cast<const DI::String*>(lEntry.Get());
                        assert(nullptr != lL);

                        lOut_Src.Copy(lLib, (std::string(*lL) + ".lib").c_str());
                        lOut_Src.Copy(lLib, (std::string(*lL) + ".pdb").c_str());
                    }
                }
            }
        }

        void Build::Test(const char* aC)
        {
            for (const auto& lEntry : mProcessors.mInternal)
            {
                assert(nullptr != lEntry);

                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                for (const auto& lEntry : mTests.mInternal)
                {
                    assert(nullptr != lEntry);

                    auto lT = dynamic_cast<const DI::String*>(lEntry.Get());
                    assert(nullptr != lT);

                    std::string lOutDir = (*lP == "x86") ? aC : std::string(*lP) + "\\" + aC;

                    Proc::Process lProcess(lOutDir.c_str(), (std::string(*lT) + ".exe").c_str());

                    lProcess.AddArgument("Groups+=Auto");

                    lProcess.Run(TEST_ALLOWED_TIME_ms);

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
