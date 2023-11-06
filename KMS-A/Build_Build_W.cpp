
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

KMS_RESULT_STATIC(RESULT_COMPILATION_FAILED);
KMS_RESULT_STATIC(RESULT_INNO_SETUP_FAILED);
KMS_RESULT_STATIC(RESULT_MAKECAB_FAILED);
KMS_RESULT_STATIC(RESULT_SIGNTOOL_FAILED);
KMS_RESULT_STATIC(RESULT_TEST_FAILED);

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define INNO_SETUP_FOLDER ("Inno Setup 6")
#define MSBUILD_FOLDER    ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")
#define WDK_TOOL_FOLDER   ("Windows Kits\\10\\bin\\10.0.19401\\x64")

#define INNO_SETUP_ALLOWED_TIME_ms (1000 * 60 *  5) // 5 minutes
#define MAKECAB_ALLOWED_TIME_ms    (1000 * 60 *  5) //  5 minutes
#define MSBUILD_ALLOWED_TIME_ms    (1000 * 60 * 10) // 10 minutes
#define SIGNTOOL_ALLOWED_TIME_ms   (1000 * 60 * 10) // 10 minutes
#define TEST_ALLOWED_TIME_ms       (1000 * 60 *  5) //  5 minutes

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Build::CERTIFICAT_SHA1_DEFAULT = "TODO";
        const char* Build::EXPORT_FOLDER_DEFAULT   = "K:\\Export";

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

            lProcess.Run(MSBUILD_ALLOWED_TIME_ms);

            auto lRet = lProcess.GetExitCode();
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_COMPILATION_FAILED, "The compilation failed", lProcess.GetCmdLine());
        }

        void Build::CreateInstaller()
        {
            auto lCT = new Dbg::Stats_Timer("CreateInstallerTime.Processor");

            for (const auto& lEntry : mProcessors.mInternal)
            {
                lCT->Start();

                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                CreateInstaller(*lP);

                lCT->Stop();
            }
        }

        void Build::Package_Components(const char* aC, const char* aP)
        {
            assert(nullptr != aC);
            assert(nullptr != aP);

            std::string lC(aC);
            std::string lP(aP);

            std::string lCP(aC);

            lCP += "_";
            lCP += lP;

            std::string lOutDir = (lP == "x86") ? aC : lP + "\\" + lC;

            File::Folder lOut_Src(lOutDir.c_str());

            if (!mBinaries.IsEmpty())
            {
                File::Folder lBin(mTmp_Binaries, lCP.c_str());

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

            if (!mDrivers.IsEmpty())
            {
                File::Folder lDrv(mTmp_Drivers, lCP.c_str());

                lDrv.Create();

                for (const auto& lEntry : mDrivers.mInternal)
                {
                    assert(nullptr != lEntry);

                    auto lD = dynamic_cast<const DI::String*>(lEntry.Get());
                    assert(nullptr != lD);

                    char lCab[PATH_LENGTH];

                    sprintf_s(lCab, "disk1\\%s.cab", lD->Get());

                    CreateDriverCab(lD->Get(), lCab);

                    File::Folder::CURRENT.Copy(lDrv, lCab);
                }
            }

            if (!mLibraries.IsEmpty())
            {
                File::Folder lLib(mTmp_Libraries, lCP.c_str());

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

        void Build::Test(const char* aC, const char* aP)
        {
            assert(nullptr != aC);
            assert(nullptr != aP);

            std::string lP(aP);

            std::string lOutDir = (lP == "x86") ? aC : lP + "\\" + aC;

            for (const auto& lEntry : mTests.mInternal)
            {
                assert(nullptr != lEntry);

                auto lT = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lT);

                Proc::Process lProcess(lOutDir.c_str(), (std::string(*lT) + ".exe").c_str());

                lProcess.AddArgument("Groups+=Auto");

                lProcess.Run(TEST_ALLOWED_TIME_ms);

                auto lRet = lProcess.GetExitCode();
                KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_TEST_FAILED, "The test failed", lProcess.GetCmdLine());
            }
        }

        void Build::CreateDriverCab(const char* aC, const char* aCabFile)
        {
            assert(nullptr != aC);
            assert(nullptr != aCabFile);

            char lDDF[PATH_LENGTH];

            sprintf_s(lDDF, "%s.ddf", aC);

            Proc::Process lP0(File::Folder::NONE, "makecab.exe");

            lP0.AddArgument("-f");
            lP0.AddArgument(lDDF);

            lP0.Run(MAKECAB_ALLOWED_TIME_ms);

            auto lRet = lP0.GetExitCode();
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_MAKECAB_FAILED, "Cannot create the cabinet file", lP0.GetCmdLine());

            Proc::Process lP1(File::Folder(File::Folder::PROGRAM_FILES_X86, WDK_TOOL_FOLDER), "signtool.exe");

            lP1.AddArgument("sign");
            lP1.AddArgument("/fd");
            lP1.AddArgument("sha256");
            lP1.AddArgument("/sha1");
            lP1.AddArgument(mCertificatSHA1);
            lP1.AddArgument(aCabFile);

            lP1.Run(SIGNTOOL_ALLOWED_TIME_ms);

            lRet = lP1.GetExitCode();
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_SIGNTOOL_FAILED, "Cannot sign the cabinet file", lP1.GetCmdLine());
        }

        void Build::CreateInstaller(const char* aP)
        {
            char lFileName[PATH_LENGTH];

            sprintf_s(lFileName, "Product_%s.iss", aP);
            if (File::Folder::CURRENT.DoesFileExist(lFileName))
            {
                Proc::Process lP0(File::Folder(File::Folder::PROGRAM_FILES_X86, INNO_SETUP_FOLDER), "Compil32.exe");

                lP0.AddArgument("/cc");
                lP0.AddArgument(lFileName);

                lP0.Run(INNO_SETUP_ALLOWED_TIME_ms);

                auto lRet = lP0.GetExitCode();
                KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_INNO_SETUP_FAILED, "Inno Setup failed", lP0.GetCmdLine());

                auto lProduct = mProduct.Get();
                auto lMa = mVersion.GetMajor();
                auto lMi = mVersion.GetMinor();
                auto lBu = mVersion.GetBuild();
                auto lType = mVersion.GetType();
                if (0 == strlen(lType))
                {
                    sprintf_s(lFileName, "%s_%u.%u.%u_%s.exe", lProduct, lMa, lMi, lBu, aP);
                }
                else
                {
                    sprintf_s(lFileName, "%s_%u.%u.%u-%s_%s.exe", lProduct, lMa, lMi, lBu, lType, aP);
                }

                File::Folder lInstaller(File::Folder::Id::CURRENT, "Installer");

                lInstaller.Copy(mProductFolder, lFileName);
            }
        }

    }
}
