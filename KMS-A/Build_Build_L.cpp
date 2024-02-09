
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Proc/Process.h>

#include <KMS/Build/Build.h>

KMS_RESULT_STATIC(RESULT_SH_FAILED);
KMS_RESULT_STATIC(RESULT_TEST_FAILED);

#define FILE_EXT_A   ".a"
#define FILE_EXT_DEB ".deb"
#define FILE_EXT_SH  ".sh"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define PACKAGES_FOLDER ("Packages")

#define SH_ALLOWED_TIME_ms   (1000 * 60 * 5) // 5 minutes
#define TEST_ALLOWED_TIME_ms (1000 * 60 * 5) // 5 minutes

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_OS_PACKAGES("LinuxPackages += {Name}");

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void DEB_FileName(const char* aPackage, const KMS::Version& aVersion, char* aOut, unsigned int aOutSize_byte);
static bool SH_FileName (const char* aProcessor, char* aOut, unsigned int aOutSize_byte);

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Build::EXPORT_FOLDER_DEFAULT = "{$HOME}/Export";

        void Build::Construct_OSDep()
        {
            mPackages.SetCreator(DI::String_Expand::Create);

            AddEntry("LinuxPackages", &mPackages, false, &MD_OS_PACKAGES);

            mExportFolder = File::Folder(File::Folder::Id::HOME, "Export");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::CreateInstaller(const char* aP)
        {
            char lFileName[PATH_LENGTH];

            if (SH_FileName(aP, lFileName, sizeof(lFileName)))
            {
                Proc::Process lP0(File::Folder::CURRENT, lFileName);

                lP0.Run(SH_ALLOWED_TIME_ms);

                auto lRet = lP0.GetExitCode();
                KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_SH_FAILED, "Script failed", lP0.GetCmdLine());

                for (auto lPackage : mPackages.mInternal)
                {
                    auto lString = dynamic_cast<const DI::String*>(lPackage.Get());
                    assert(nullptr != lString);

                    DEB_FileName(lString->GetString().c_str(), mVersion, lFileName, sizeof(lFileName));

                    File::Folder lPackages(File::Folder::Id::CURRENT, PACKAGES_FOLDER);

                    lPackages.Copy(mProductFolder, lFileName);
                }
            }
        }

        void Build::Package_Components(const char* aC, const char* aP)
        {
            assert(nullptr != aC);
            assert(nullptr != aP);

            char lCP[PATH_LENGTH];

            sprintf_s(lCP, "%s_%s", aC, aP);

            File::Folder lBin(mTmp_Binaries , lCP);
            File::Folder lLib(mTmp_Libraries, lCP);

            File::Folder lBin_Src((std::string("Binaries/" ) + lCP).c_str());
            File::Folder lLib_Src((std::string("Libraries/") + lCP).c_str());

            lBin.Create();
            lLib.Create();

            for (const auto& lEntry : mBinaries.mInternal)
            {
                auto lB = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lB);

                lBin_Src.Copy(lBin, lB->Get());
            }

            for (const auto& lEntry : mLibraries.mInternal)
            {
                auto lL = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lL);

                lLib_Src.Copy(lLib, (lL->GetString() + FILE_EXT_A).c_str());
            }
        }

        void Build::Test(const char* aC, const char* aP)
        {
            assert(nullptr != aC);
            assert(nullptr != aP);

            std::string lOutDir = std::string("Binaries/") + aC + "_" + aP;

            for (const auto& lEntry : mTests.mInternal)
            {
                auto lT = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lT);

                Proc::Process lP(lOutDir.c_str(), lT->Get());

                lP.AddArgument("Groups+=Auto");

                lP.Run(TEST_ALLOWED_TIME_ms);

                char lMsg[64 + NAME_LENGTH];
                sprintf(lMsg, "\"%s\" failed", lT->Get());
                KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), RESULT_TEST_FAILED, lMsg, lP.GetCmdLine());
            }
        }

    }
}

using namespace KMS;

// Static functions
// /////////////////////////////////////////////////////////////////////////

void DEB_FileName(const char* aPackage, const KMS::Version& aVersion, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aPackage);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    auto lMa = aVersion.GetMajor();
    auto lMi = aVersion.GetMinor();
    auto lBu = aVersion.GetBuild();
    
    sprintf(aOut, "%s_%u.%u-%u" FILE_EXT_DEB, aPackage, lMa, lMi, lBu);
}

bool SH_FileName(const char* aProcessor, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aProcessor);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    sprintf(aOut, "Product_%s" FILE_EXT_SH, aProcessor);

    return File::Folder::CURRENT.DoesFileExist(aOut);
}
