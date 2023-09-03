
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

#include <KMS/Build/Build.h>

KMS_RESULT_MODULE("Build_Build");
KMS_RESULT_SATIC_CODE(RESULT_TEST_FAILED, CODE_ERROR, "An test component execution failed");

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define TEST_ALLOWED_TIME_ms (1000 * 60 * 5) // 5 minutes

namespace KMS
{
    namespace Build
    {

        // Private
        // //////////////////////////////////////////////////////////////////

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

                lLib_Src.Copy(lLib, (lL->mInternal + ".a").c_str());
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
                KMS_EXCEPTION_ASSERT_2(0 == lP.GetExitCode(), RESULT_TEST_FAILED, lMsg, lP.GetCmdLine());
            }
        }

    }
}

