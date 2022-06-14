
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Process.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{

    class Process
    {

    public:

        Process(const File::Folder& aFolder, const char* aExecutable);

        ~Process();

        void AddArgument(const char * aArgument);

        const char* GetCmdLine() const;

        void SetWorkingDirectory(const char* aWD);

        int Run();

    private:

        Process(const Process &);

        const Process & operator = (const Process &);

        int Run_Internal();

        StringList   mArguments;
        char       * mCmdLine;
        std::string  mExecutable;
        File::Folder mFolder;
        std::string  mWorkingDirectory;

    };

}
