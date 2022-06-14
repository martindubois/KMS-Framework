
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Process_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Process.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CloseHandles(PROCESS_INFORMATION* aInfo);

static int WaitAndClose(PROCESS_INFORMATION* aInfo);

namespace KMS
{

    // Pirvate
    // //////////////////////////////////////////////////////////////////////

    int Process::Run_Internal()
    {
        assert(NULL != mCmdLine);

        const char* lWD = NULL;

        if (0 < mWorkingDirectory.size())
        {
            lWD = mWorkingDirectory.c_str();
        }

        PROCESS_INFORMATION lInfo;
        STARTUPINFO lStart;

        memset(&lStart, 0, sizeof(lStart));

        lStart.cb = sizeof(lStart);

        BOOL lRetB = CreateProcess(NULL, mCmdLine, NULL, NULL, TRUE, 0, NULL, lWD, &lStart, &lInfo);
        if (!lRetB)
        {
            KMS_EXCEPTION_WITH_INFO(PROCESS_START, "CreateProcess failed", mCmdLine);
        }

        return WaitAndClose(&lInfo);
    }

}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void CloseHandles(PROCESS_INFORMATION* aInfo)
{
    assert(NULL != aInfo);

    BOOL lRet = CloseHandle(aInfo->hProcess);
    assert(lRet);

    lRet = CloseHandle(aInfo->hThread);
    assert(lRet);
}

int WaitAndClose(PROCESS_INFORMATION* aInfo)
{
    assert(NULL != aInfo);

    DWORD lRet = WaitForSingleObject(aInfo->hProcess, 5 * 60 * 1000);
    if (WAIT_OBJECT_0 != lRet)
    {
        CloseHandles(aInfo);

        KMS_EXCEPTION_WITH_INFO(PROCESS_TIMEOUT, "WaitForSingleObject failed", lRet);
    }

    DWORD lResult;

    if (!GetExitCodeProcess(aInfo->hProcess, &lResult))
    {
        CloseHandles(aInfo);

        KMS_EXCEPTION(PROCESS_EXIT_CODE, "GetExitCodeProcess failed");
    }

    CloseHandles(aInfo);

    return lResult;
}
