
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Proc_Process_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void ChangeDirectory(const char* aD);

namespace KMS
{
    namespace Proc
    {

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
                    KMS_EXCEPTION(PROC_ACCESS_FAILED, "Cannot retrive the current workding directory", mCmdLine);
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
            case -1: KMS_EXCEPTION(PROC_START_FAILED, "fork failed", mCmdLine);

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
                            KMS_EXCEPTION(PROC_ACCESS_FAILED, "chdir failed", mWorkingDirectory.c_str());
                        }
                    }

                    auto lRet = execv(lPath, const_cast<char **>(lVector));
                    assert(-1 == lRet);

                    KMS_EXCEPTION(PROC_START_FAILED, "execv failed", mCmdLine);
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
        KMS_EXCEPTION(PROC_ACCESS_FAILED, "chdir failed", aD);
    }
}
