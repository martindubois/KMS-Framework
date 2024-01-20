
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Proc_Process_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== System =============================================================
#include <sys/wait.h>

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

KMS_RESULT_STATIC(RESULT_ACCESS_FAILED);
KMS_RESULT_STATIC(RESULT_START_FAILED);

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void ChangeDirectory(const char* aD);

namespace KMS
{
    namespace Proc
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool Process::IsRunning()
        {
            bool lResult = false;

            if (0 < mHandle)
            {
                int lStatus;

                auto lRet = waitpid(mHandle, &lStatus, WNOHANG);
                KMS_EXCEPTION_ASSERT(0 <= lRet, RESULT_ACCESS_FAILED, "waitpid failed", lRet);

                if (0 == lRet)
                {
                    lResult = true;
                }
                else
                {
                    mExitCode = lStatus;
                    mHandle = 0;
                }
            }

            return lResult;
        }

        void Process::Detach() { mHandle = 0; }

        void Process::Terminate()
        {
            if (0 < mHandle)
            {
                auto lRet = kill(mHandle, SIGINT);
                KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_ACCESS_FAILED, "kill failed", lRet);
            }
        }

        void Process::Wait(unsigned int aTimeout_ms)
        {
            unsigned int lTimeout_ms = aTimeout_ms;

            while (IsRunning())
            {
                KMS_EXCEPTION_ASSERT(0 < lTimeout_ms, RESULT_TIMEOUT, "The process did not complete in the allowed time", aTimeout_ms);

                unsigned int lSleep_ms = 100;
                if (lTimeout_ms < lSleep_ms)
                {
                    lSleep_ms = lTimeout_ms;
                }

                usleep(lSleep_ms * 1000);

                lTimeout_ms -= lSleep_ms;
            }
        }

        // Pirvate
        // //////////////////////////////////////////////////////////////////

        void Process::Run_Internal(unsigned int)
        {
            assert(nullptr != mCmdLine);

            auto lCD = 0 < mWorkingDirectory.size();

            char lDir[PATH_LENGTH];

            if (lCD)
            {
                if (NULL == getcwd(lDir, sizeof(lDir)))
                {
                    KMS_EXCEPTION(RESULT_ACCESS_FAILED, "Cannot retrive the current workding directory", mCmdLine);
                }

                ChangeDirectory(mWorkingDirectory.c_str());
            }

            mExitCode = system(mCmdLine);

            if (lCD)
            {
                ChangeDirectory(lDir);
            }
        }

        void Process::Start_Internal()
        {
            auto lPId = fork();
            switch (lPId)
            {
            case -1: KMS_EXCEPTION(RESULT_START_FAILED, "fork failed", mCmdLine);

            case 0:
                int lResult;
                lResult = __LINE__;
                try
                {
                    unsigned int lIndex = 1;
                    char lPath[PATH_LENGTH];
                    const char* lVector[mArguments.size() + 2];

                    mFolder.GetPath(mExecutable.c_str(), lPath, sizeof(lPath));

                    lVector[0] = lPath;

                    for (const auto& lA : mArguments)
                    {
                        lVector[lIndex] = lA.c_str(); lIndex++;
                    }

                    lVector[lIndex] = nullptr;

                    if (0 < mWorkingDirectory.size())
                    {
                        if (0 != chdir(mWorkingDirectory.c_str()))
                        {
                            KMS_EXCEPTION(RESULT_ACCESS_FAILED, "chdir failed", mWorkingDirectory.c_str());
                        }
                    }

                    auto lRet = execv(lPath, const_cast<char **>(lVector));
                    assert(-1 == lRet);

                    KMS_EXCEPTION(RESULT_START_FAILED, "execv failed", mCmdLine);
                }
                KMS_CATCH_RESULT(lResult);

                exit(lResult);
                break;

            default: mHandle = lPId;
            }
        }

    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void ChangeDirectory(const char* aD)
{
    assert(nullptr != aD);

    if (0 != chdir(aD))
    {
        KMS_EXCEPTION(RESULT_ACCESS_FAILED, "chdir failed", aD);
    }
}
