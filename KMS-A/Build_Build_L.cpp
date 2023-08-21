
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

#include <KMS/Build/Build.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define TEST_ALLOWED_TIME_ms (1000 * 60 * 5) // 5 minutes

namespace KMS
{
    namespace Build
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Package_Components(const char* aC)
        {
            File::Folder lBin(mTmp_Binaries , aC);
            File::Folder lLib(mTmp_Libraries, aC);

            File::Folder lBin_Src((std::string("Binaries/" ) + aC).c_str());
            File::Folder lLib_Src((std::string("Libraries/") + aC).c_str());

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

        void Build::Test(const char* aC)
        {
            for (const auto& lEntry : mTests.mInternal)
            {
                auto lT = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lT);

                Proc::Process lP((std::string("Binaries/") + aC).c_str(), lT->Get());

                lP.AddArgument("Groups+=Auto");

                lP.Run(TEST_ALLOWED_TIME_ms);

                char lMsg[64 + NAME_LENGTH];
                sprintf(lMsg, "\"%s\" failed", lT->Get());
                KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), BUILD_TEST_FAILED, lMsg, lP.GetCmdLine());
            }
        }

    }
}

