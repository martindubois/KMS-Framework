
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Process_Process_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== Includes ===========================================================
#include <KMS/Process/Process.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void ChangeDirectory(const char* aD);

namespace KMS
{
    namespace Process
    {

        // Pirvate
        // //////////////////////////////////////////////////////////////////

        void Process::Run_Internal(unsigned int)
        {
            assert(NULL != mCmdLine);

            bool lCD = 0 < mWorkingDirectory.size();

            char lDir[PATH_LENGTH];

            if (lCD)
            {
                if (NULL == getcwd(lDir, sizeof(lDir)))
                {
                    KMS_EXCEPTION(FOLDER_ACCESS, "getcwd failed");
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
            pid_t lPId = fork();
            switch (lPId)
            {
            case -1: KMS_EXCEPTION_WITH_INFO(PROCESS_START, "fork failed", mCmdLine);

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

                    for (std::string& lA : mArguments)
                    {
                        lVector[lIndex] = lA.c_str(); lIndex++;
                    }

                    lVector[lIndex] = NULL;

                    if (0 < mWorkingDirectory.size())
                    {
                        if (0 != chdir(mWorkingDirectory.c_str()))
                        {
                            KMS_EXCEPTION_WITH_INFO(FOLDER_ACCESS, "chdir failed", mWorkingDirectory.c_str());
                        }
                    }

                    int lRet = execv(lPath, const_cast<char **>(lVector));
                    assert(-1 == lRet);

                    KMS_EXCEPTION_WITH_INFO(PROCESS_START, "execv failed", mCmdLine);
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
    assert(NULL != aD);

    if (0 != chdir(aD))
    {
        KMS_EXCEPTION_WITH_INFO(FOLDER_ACCESS, "chdir failed", aD);
    }
}
