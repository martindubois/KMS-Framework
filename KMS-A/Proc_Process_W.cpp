
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Proc_Process_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

namespace KMS
{
    namespace Proc
    {
        // Public
        // //////////////////////////////////////////////////////////////////

        bool Process::IsRunning()
        {
            if (0 == mHandle)
            {
                return false;
            }

            DWORD lExitCode;
            auto lHandle = reinterpret_cast<HANDLE>(mHandle);

            if (!GetExitCodeProcess(lHandle, &lExitCode))
            {
                mExitCode = EXIT_CODE_ERROR;
                KMS_EXCEPTION(PROC_ACCESS_FAILED, "Cannot retrieve the process's exit code", mCmdLine);
            }

            if (STILL_ACTIVE == lExitCode)
            {
                return true;
            }

            Detach();

            mExitCode = lExitCode;

            return false;
        }

        void Process::Detach()
        {
            assert(0 != mHandle);

            auto lHandle = reinterpret_cast<HANDLE>(mHandle);

            mHandle = 0;

            auto lRetB = CloseHandle(lHandle);
            assert(lRetB);
        }

        void Process::Terminate()
        {
            assert(0 != mHandle);

            auto lHandle = reinterpret_cast<HANDLE>(mHandle);

            if (!TerminateProcess(lHandle, 0))
            {
                mExitCode = EXIT_CODE_ERROR;
                KMS_EXCEPTION(PROC_KILL_FAILED, "Cannot terminat the process", "");
            }

            Detach();

            mExitCode = EXIT_CODE_TERMINATED;
        }

        void Process::Wait(unsigned int aTimeout_ms)
        {
            assert(0 != mHandle);

            auto lHandle = reinterpret_cast<HANDLE>(mHandle);

            if (WAIT_OBJECT_0 != WaitForSingleObject(lHandle, aTimeout_ms))
            {
                KMS_EXCEPTION(PROC_TIMEOUT, "The process did not complete in the allowed time", aTimeout_ms);
            }

            DWORD lExitCode;

            if (!GetExitCodeProcess(lHandle, &lExitCode))
            {
                mExitCode = EXIT_CODE_ERROR;
                KMS_EXCEPTION(PROC_ACCESS_FAILED, "Cannot retrive the process's exit code", aTimeout_ms);
            }

            Detach();

            mExitCode = lExitCode;
        }

        // Pirvate
        // //////////////////////////////////////////////////////////////////

        void Process::Run_Internal(unsigned int aTimeout_ms)
        {
            Start_Internal();

            Wait(aTimeout_ms);
        }

        void Process::Start_Internal()
        {
            assert(nullptr != mCmdLine);

            const char* lWD = nullptr;

            if (0 < mWorkingDirectory.size())
            {
                lWD = mWorkingDirectory.c_str();
            }

            if (0 == mVerb.size())
            {
                PROCESS_INFORMATION lInfo;

                STARTUPINFO lStart;

                memset(&lStart, 0, sizeof(lStart));

                lStart.cb = sizeof(lStart);

                auto lRetB = CreateProcess(NULL, mCmdLine, NULL, NULL, FALSE, 0, NULL, lWD, &lStart, &lInfo);
                if (!lRetB)
                {
                    mExitCode = EXIT_CODE_ERROR;
                    KMS_EXCEPTION(PROC_START_FAILED, "Cannot create the process", mCmdLine);
                }

                lRetB = CloseHandle(lInfo.hThread);
                assert(lRetB);

                mHandle = reinterpret_cast<uint64_t>(lInfo.hProcess);
            }
            else
            {
                char lPath[PATH_LENGTH];

                mFolder.GetPath(mExecutable.c_str(), lPath, sizeof(lPath));

                SHELLEXECUTEINFO lInfo;

                memset(&lInfo, 0, sizeof(lInfo));

                lInfo.cbSize = sizeof(lInfo);
                lInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI | SEE_MASK_NO_CONSOLE;
                lInfo.lpVerb = mVerb.c_str();
                lInfo.lpFile = lPath;
                lInfo.lpDirectory = lWD;
                lInfo.nShow = SW_SHOWDEFAULT;

                if (!ShellExecuteEx(&lInfo))
                {
                    mExitCode = EXIT_CODE_ERROR;
                    KMS_EXCEPTION(PROC_START_FAILED, "Cannot execute the shell operation", lPath);
                }

                mHandle = reinterpret_cast<uint64_t>(lInfo.hProcess);
            }
        }

    }
}
