
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Console/Redirection.h
// Library   KMS-A

#pragma once

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace Console
    {

        class Redirection final
        {

        public:

            static const File::Folder FOLDER_DEFAULT;

            static File::Folder sFolder;

            enum class What
            {
                WHAT_STDERR = 0,
                WHAT_STDOUT,

                WHAT_ALL,

                QTY
            };

            enum class To
            {
                TO_FILE,
                TO_NULL,

                QTY
            };

            Redirection(What aWhat = What::WHAT_ALL, To aTo = To::TO_NULL);

            ~Redirection();

            void Redirect();

            void Restore();

        private:

            void Construct_Begin();
            void Construct_End(What aWhat, To aTo);

            void Destruct();

            int OpenFileIfNeeded();
            int OpenNullIfNeeded();

            void Redirect_OSDep();
            void Restore_OSDep ();

            void Save(What aWhat);
            void Save_OSDep(What aWhat);

            int mRedirect[2];
            int mRestore [2];

            #ifdef _KMS_LINUX_
                FILE* mFiles[2];
            #endif

            #ifdef _KMS_WINDOWS_

                void OpenOSFile(unsigned int aFile);

                HANDLE mFiles  [2];
                int    mFileNos[2];

                HANDLE mRedirect_W[2];
                HANDLE mRestore_W [2];

            #endif

        };

    }
}
