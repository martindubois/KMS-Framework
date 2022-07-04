
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Proc_Process.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

namespace KMS
{
    namespace Proc
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const int Process::EXIT_CODE_DETACHED    = 0xfffffff4;
        const int Process::EXIT_CODE_ERROR       = 0xfffffff5;
        const int Process::EXIT_CODE_NOT_STARTED = 0xfffffff6;
        const int Process::EXIT_CODE_RUNNING     = 0xfffffff7;
        const int Process::EXIT_CODE_TERMINATED  = 0xfffffff8;

        Process::Process(const File::Folder& aFolder, const char* aExecutable)
            : mCmdLine(NULL)
            , mExecutable(aExecutable)
            , mExitCode(EXIT_CODE_NOT_STARTED)
            , mFolder(aFolder)
            , mHandle(0)
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

        int Process::GetExitCode() const { return mExitCode; }

        void Process::SetVerb(const char* aV) { assert(NULL != aV); mVerb = aV; }

        void Process::SetWorkingDirectory(const char* aWD) { assert(NULL != aWD); mWorkingDirectory = aWD; }

        void Process::Run(unsigned int aTimeout_ms)
        {
            BuildCommandLine();

            mExitCode = EXIT_CODE_RUNNING;

            Run_Internal(aTimeout_ms);
        }

        void Process::Start()
        {
            BuildCommandLine();

            mExitCode = EXIT_CODE_RUNNING;

            Start_Internal();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Process::BuildCommandLine()
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
        }

    }
}
