
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Proc/Process.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Proc
    {

        class Process final
        {

        public:

            static const int EXIT_CODE_DETACHED;
            static const int EXIT_CODE_ERROR;
            static const int EXIT_CODE_NOT_STARTED;
            static const int EXIT_CODE_RUNNING;
            static const int EXIT_CODE_TERMINATED;

            Process(const File::Folder& aFolder, const char* aExecutable);

            ~Process();

            void AddArgument(const char * aArgument);

            const char* GetCmdLine() const;

            int GetExitCode() const;

            bool IsRunning();

            void SetVerb(const char* aV);

            void SetWorkingDirectory(const char* aWD);

            void Detach();

            // LIMITATION Darwin and Linux implementations ignore the aTimeout_ms
            //            argument.
            void Run(unsigned int aTimeout_ms);

            void Start();

            void Terminate();

            void Wait(unsigned int aTimeout_ms);

        private:

            NO_COPY(Process);

            void BuildCommandLine();

            void Run_Internal(unsigned int aTimeout_ms);

            void Start_Internal();

            StringList_ASCII mArguments;
            char           * mCmdLine;
            String_ASCII     mExecutable;
            int              mExitCode;
            File::Folder     mFolder;
            uint64_t         mHandle;
            String_ASCII     mVerb;
            String_ASCII     mWorkingDirectory;

        };

    }
}
