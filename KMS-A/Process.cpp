
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Process.cpp

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

    // Public
    // //////////////////////////////////////////////////////////////////////

    Process::Process(const File::Folder& aFolder, const char* aExecutable)
        : mCmdLine(NULL)
        , mExecutable(aExecutable)
        , mFolder(aFolder)
    {
        assert(NULL != aExecutable);
    }

    Process::~Process()
    {
        if (NULL != mCmdLine)
        {
            delete[] mCmdLine;
        }
    }

    void Process::AddArgument(const char* aArgument)
    {
        assert(NULL != aArgument);

        mArguments.push_back(aArgument);
    }

    const char* Process::GetCmdLine() const { return mCmdLine; }

    int Process::Run()
    {
        char lExec[MAX_PATH];
        mFolder.GetPath(mExecutable.c_str(), lExec, sizeof(lExec));

        std::string lCmdLine = "\"";
        lCmdLine += lExec;
        lCmdLine += "\"";

        for (std::string lA : mArguments)
        {
            lCmdLine += " \"" + lA + "\"";
        }

        size_t lLen = lCmdLine.size() + 1;

        mCmdLine = new char[lLen];
        assert(NULL != mCmdLine);

        memcpy(mCmdLine, lCmdLine.c_str(), lLen * sizeof(char));

        PROCESS_INFORMATION lInfo;
        STARTUPINFO lStart;

        memset(&lStart, 0, sizeof(lStart));

        lStart.cb = sizeof(lStart);

        BOOL lRetB = CreateProcess(NULL, mCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &lStart, &lInfo);
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
