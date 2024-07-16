
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Console/HumanScript.h>
#include <KMS/Proc/Process.h>
#include <KMS/Dbg/Stats_Timer.h>

#include <KMS/Build/Build.h>

KMS_RESULT_STATIC(RESULT_COMPILATION_FAILED);
KMS_RESULT_STATIC(RESULT_DOXYGEN_FAILED);
KMS_RESULT_STATIC(RESULT_INNO_SETUP_FAILED);
KMS_RESULT_STATIC(RESULT_INNO_SETUP_MISSING);
KMS_RESULT_STATIC(RESULT_MAKECAB_FAILED);
KMS_RESULT_STATIC(RESULT_SIGNTOOL_FAILED);
KMS_RESULT_STATIC(RESULT_TEST_FAILED);
KMS_RESULT_STATIC(RESULT_VISUAL_STUDIO_MISSING);
KMS_RESULT_STATIC(RESULT_WDK_MISSING);

#define FILE_EXT_CAB ".cab"
#define FILE_EXT_DDF ".ddf"
#define FILE_EXT_EXE ".exe"
#define FILE_EXT_ISS ".iss"
#define FILE_EXT_LIB ".lib"
#define FILE_EXT_PDB ".pdb"
#define FILE_EXT_SLN ".sln"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DISK1_FOLDER      ("disk1")
#define DOXYGEN_EXE       ("doxygen.exe")
#define DOXYGEN_FOLDER    ("doxygen\\bin")
#define INNO_SETUP_EXE    ("Compil32" FILE_EXT_EXE)
#define INNO_SETUP_FOLDER ("Inno Setup 6")
#define INSTALLER_FOLDER  ("Installer")
#define MSBUILD_EXE       ("MSBuild" FILE_EXT_EXE)
#define MSBUILD_FOLDER    ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")
#define SIGNTOOL_EXE      ("signtool" FILE_EXT_EXE)
#define WDK_TOOL_FOLDER   ("Windows Kits\\10\\bin\\10.0.22621.0\\x64")

#define DOXYGEN_ALLOWED_TIME_ms    (1000 * 60 *  5) //  5 minutes
#define INNO_SETUP_ALLOWED_TIME_ms (1000 * 60 * 10) // 10 minutes
#define MAKECAB_ALLOWED_TIME_ms    (1000 * 60 *  5) //  5 minutes
#define MSBUILD_ALLOWED_TIME_ms    (1000 * 60 * 10) // 10 minutes
#define SIGNTOOL_ALLOWED_TIME_ms   (1000 * 60 * 10) // 10 minutes
#define TEST_ALLOWED_TIME_ms       (1000 * 60 *  5) //  5 minutes

// Constants
// /////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_CERTIFICAT_SHA1("CertificatSHA1 = {SHA1}");

// Static function declaration
// /////////////////////////////////////////////////////////////////////////

static void CAB_FileName(const char* aDriver   , const char* aFolder, char* aOut, unsigned int aOutSize_byte);
static bool DDF_FileName(const char* aDriver   ,                      char* aOut, unsigned int aOutSize_byte);
static bool ISS_FileName(const char* aProcessor,                      char* aOut, unsigned int aOutSize_byte);

static const char* SLN_FILE_NAME = "Solution" FILE_EXT_SLN;

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Build::CERTIFICAT_SHA1_DEFAULT = "B71CF3BCD4E228FEFD58B2FE3353EF31106C1754";
        const char* Build::EXPORT_FOLDER_DEFAULT   = "K:\\Export";

        void Build::Construct_OSDep()
        {
            mCertificatSHA1 = CERTIFICAT_SHA1_DEFAULT;
            mExportFolder   = EXPORT_FOLDER_DEFAULT;

            Ptr_OF<DI::Object> lEntry(&mCertificatSHA1, false);

            AddEntry("CertificatSHA1", lEntry, &MD_CERTIFICAT_SHA1);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile_VisualStudio(const char* aC)
        {
            auto lCT = new Dbg::Stats_Timer("CompileTime.Processor");

            for (const auto& lEntry : mProcessors.mInternal)
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
            File::Folder lBin(File::Folder::PROGRAM_FILES, MSBUILD_FOLDER);

            Proc::Process lProcess(lBin, MSBUILD_EXE);

            lProcess.AddArgument(SLN_FILE_NAME);

            if (mDoNotClean.Get())
            {
                lProcess.AddArgument("/target:build");
            }
            else
            {
                lProcess.AddArgument("/target:rebuild");
            }

            lProcess.AddArgument((std::string("/Property:Configuration=") + aC).c_str());
            lProcess.AddArgument(("/property:Platform=" + std::string(aP)).c_str());

            lProcess.Run(MSBUILD_ALLOWED_TIME_ms);

            auto lRet = lProcess.GetExitCode();
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_COMPILATION_FAILED, "The compilation failed", lProcess.GetCmdLine());
        }

        void Build::ExecuteDoxygen(const char* aFileName)
        {
            File::Folder lBin(File::Folder::PROGRAM_FILES, DOXYGEN_FOLDER);

            Proc::Process lP0(lBin, DOXYGEN_EXE);

            lP0.AddArgument(aFileName);

            lP0.Run(DOXYGEN_ALLOWED_TIME_ms);

            auto lRet = lP0.GetExitCode();
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_DOXYGEN_FAILED, "doxygen failed", lP0.GetCmdLine());
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

                    lOut_Src.Copy(lBin, (std::string(*lB) + FILE_EXT_EXE).c_str());
                    lOut_Src.Copy(lBin, (std::string(*lB) + FILE_EXT_PDB).c_str());
                }
            }

            if ((!mDrivers.IsEmpty()) && (0 != strcmp("Debug", aC)))
            {
                File::Folder lDrv(mTmp_Drivers, lCP.c_str());

                lDrv.Create();

                for (const auto& lEntry : mDrivers.mInternal)
                {
                    assert(nullptr != lEntry);

                    auto lD = dynamic_cast<const DI::String*>(lEntry.Get());
                    assert(nullptr != lD);

                    char lFileName[PATH_LENGTH];

                    CAB_FileName(*lD, DISK1_FOLDER, lFileName, sizeof(lFileName));

                    CreateDriverCab(lD->Get(), lFileName);

                    char lSigned[PATH_LENGTH];
                    sprintf_s(lSigned, "Signed\\%s", lD->GetString().c_str());

                    if (SignDriver(lFileName, lSigned, aC, aP))
                    {
                        File::Folder lSrcF(File::Folder::CURRENT, lSigned);

                        lSrcF.Copy(lDrv);
                    }
                    else
                    {
                        File::Folder lDisk1(File::Folder::CURRENT, DISK1_FOLDER);

                        CAB_FileName(*lD, nullptr, lFileName, sizeof(lFileName));

                        lDisk1.Copy(lDrv, lFileName);
                    }
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

                    lOut_Src.Copy(lLib, (std::string(*lL) + FILE_EXT_LIB).c_str());
                    lOut_Src.Copy(lLib, (std::string(*lL) + FILE_EXT_PDB).c_str());
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

                Proc::Process lProcess(lOutDir.c_str(), (std::string(*lT) + FILE_EXT_EXE).c_str());

                lProcess.AddArgument("Groups+=Auto");

                lProcess.Run(TEST_ALLOWED_TIME_ms);

                auto lRet = lProcess.GetExitCode();
                KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_TEST_FAILED, "The test failed", lProcess.GetCmdLine());
            }
        }

        void Build::Validate_OSDep()
        {
            if ((!mDoNotCompile) && (0 == mEmbedded.GetLength()))
            {
                File::Folder lBin(File::Folder::PROGRAM_FILES, MSBUILD_FOLDER);

                KMS_EXCEPTION_ASSERT(lBin.DoesFileExist(MSBUILD_EXE), RESULT_VISUAL_STUDIO_MISSING, "Visual Studio 2022 is not installed", "");
            }

            for (const auto& lEntry : mProcessors.mInternal)
            {
                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                char lFileName[PATH_LENGTH];

                if (ISS_FileName(*lP, lFileName, sizeof(lFileName)))
                {
                    File::Folder lBin(File::Folder::PROGRAM_FILES_X86, INNO_SETUP_FOLDER);

                    KMS_EXCEPTION_ASSERT(lBin.DoesFileExist(INNO_SETUP_EXE), RESULT_INNO_SETUP_MISSING, "Inno Setup 6.2 is not installed", "");

                    break;
                }
            }

            if (!mDrivers.IsEmpty())
            {
                File::Folder lBin(File::Folder::PROGRAM_FILES_X86, WDK_TOOL_FOLDER);
                
                KMS_EXCEPTION_ASSERT(lBin.DoesFileExist(SIGNTOOL_EXE), RESULT_WDK_MISSING, "The WDK 10.0.19401 is not installed", "");
            }

        }

        void Build::CreateDriverCab(const char* aD, const char* aCabFile)
        {
            assert(nullptr != aD);
            assert(nullptr != aCabFile);

            char lDDF[PATH_LENGTH];

            if (DDF_FileName(aD, lDDF, sizeof(lDDF)))
            {
                Proc::Process lP0(File::Folder::NONE, "makecab" FILE_EXT_EXE);

                lP0.AddArgument("-f");
                lP0.AddArgument(lDDF);

                lP0.Run(MAKECAB_ALLOWED_TIME_ms);

                auto lRet = lP0.GetExitCode();
                KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_MAKECAB_FAILED, "Cannot create the cabinet file", lP0.GetCmdLine());

                Proc::Process lP1(File::Folder(File::Folder::PROGRAM_FILES_X86, WDK_TOOL_FOLDER), SIGNTOOL_EXE);

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
        }

        void Build::CreateInstaller(const char* aP)
        {
            char lFileName[PATH_LENGTH];

            if (ISS_FileName(aP, lFileName, sizeof(lFileName)))
            {
                Proc::Process lP0(File::Folder(File::Folder::PROGRAM_FILES_X86, INNO_SETUP_FOLDER), INNO_SETUP_EXE);

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
                    sprintf_s(lFileName, "%s_%u.%u.%u_%s" FILE_EXT_EXE, lProduct, lMa, lMi, lBu, aP);
                }
                else
                {
                    sprintf_s(lFileName, "%s_%u.%u.%u-%s_%s" FILE_EXT_EXE, lProduct, lMa, lMi, lBu, lType, aP);
                }

                File::Folder lInstaller(File::Folder::Id::CURRENT, INSTALLER_FOLDER);

                lInstaller.Copy(mProductFolder, lFileName);
            }
        }

        bool Build::SignDriver(const char* aCabFile, const char* aSigned, const char* aComponent, const char* aProcessor)
        {
            assert(nullptr != aCabFile);
            assert(nullptr != aSigned);

            Console::HumanScript lHS;

            char lStep[LINE_LENGTH];

            lHS.Begin("Signing by Microsoft");
            lHS.Line("Tool - WEB navigator - https://partner.microsoft.com/en-us/dashboard/home");
            lHS.Step("- Click \"Hardware\"");
            lHS.Step("- Click \"Submit new hardware\"");

            sprintf_s(lStep, "- Enter \"%s\" as product name", aComponent);
            lHS.Step(lStep);

            sprintf_s(lStep, "- Upload the signed cabinet file (%s)", aCabFile);
            lHS.Step(lStep);

            sprintf_s(lStep, "- Select the appropriate platforms (%s)", aProcessor);
            lHS.Step(lStep);

            lHS.Step("- Click \"Submit\"");
            lHS.Step("- Wait for Microsoft to sign the driver");
            lHS.Step("- Download the Microsoft signed driver");
            lHS.Line("");

            sprintf_s(lStep, "Uncompress the downloaded file into the \"%s\" folder", aSigned);
            lHS.Step(lStep);

            lHS.Line("");

            auto lResult = lHS.Wait(Console::HumanScript::RESULT_OK, "eno");

            lHS.End();

            return Console::HumanScript::RESULT_OK == lResult;
        }

    }
}

using namespace KMS;

// Static functions
// /////////////////////////////////////////////////////////////////////////

void CAB_FileName(const char* aDriver, const char* aFolder, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aDriver);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    if (nullptr == aFolder)
    {
        sprintf_s(aOut, aOutSize_byte, "%s" FILE_EXT_CAB, aDriver);
    }
    else
    {
        sprintf_s(aOut, aOutSize_byte, "%s\\%s" FILE_EXT_CAB, aFolder, aDriver);
    }
}

bool DDF_FileName(const char* aDriver, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aDriver);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    sprintf_s(aOut, aOutSize_byte, "%s" FILE_EXT_DDF, aDriver);

    return File::Folder::CURRENT.DoesFileExist(aOut);
}

bool ISS_FileName(const char* aProcessor, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aProcessor);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    sprintf_s(aOut, aOutSize_byte, "Product_%s" FILE_EXT_ISS, aProcessor);

    return File::Folder::CURRENT.DoesFileExist(aOut);
}
