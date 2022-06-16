
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Process.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Process.h>

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

    void Process::SetWorkingDirectory(const char* aWD) { assert(NULL != aWD); mWorkingDirectory = aWD; }

    int Process::Run()
    {
        char lExec[PATH_LENGTH];

        mFolder.GetPath(mExecutable.c_str(), lExec, sizeof(lExec));

        std::string lCmdLine = "\"";
        lCmdLine += lExec;
        lCmdLine += "\"";

        for (std::string lA : mArguments)
        {
            if (std::string::npos == lA.find_first_of(' '))
            {
                lCmdLine += " " + lA;
            }
            else
            {
                lCmdLine += " \"" + lA + "\"";
            }
        }

        size_t lLen = lCmdLine.size() + 1;

        mCmdLine = new char[lLen];
        assert(NULL != mCmdLine);

        memcpy(mCmdLine, lCmdLine.c_str(), lLen * sizeof(char));

        return Run_Internal();
    }

}
