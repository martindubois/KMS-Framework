
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Console_Redirection_W.cpp

#include "Component.h"

// ===== C ==================================================================
#include <fcntl.h>
#include <io.h>

// ===== Includes ===========================================================
#include <KMS/Console/Redirection.h>
#include <KMS/OS.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define FILE_FILE (0)
#define FILE_NULL (1)

#define FILE_QTY (2)

#define HANDLE_INVALID (-1)

#define INDEX_STDERR (0)
#define INDEX_STDOUT (1)

#define INDEX_QTY (2)

#define NULL_DEV "\\\\.\\NUL"

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void RedirectIfNeeded(HANDLE aRedirect[2]);

namespace KMS
{
    namespace Console
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Redirection::Construct_Begin()
        {
            unsigned int i;

            for (i = 0; i < FILE_QTY; i++)
            {
                mFiles  [i] = INVALID_HANDLE_VALUE;
                mFileNos[i] = HANDLE_INVALID;
            }

            for (i = 0; i < INDEX_QTY; i++)
            {
                mRedirect_W[i] = INVALID_HANDLE_VALUE;
            }
        }

        void Redirection::Construct_End(What aWhat, To aTo)
        {
            HANDLE lTo = INVALID_HANDLE_VALUE;

            switch (aTo)
            {
            case To::TO_FILE: lTo = mFiles[FILE_FILE]; break;
            case To::TO_NULL: lTo = mFiles[FILE_NULL]; break;

            default: assert(false);
            }

            switch (aWhat)
            {
            case What::WHAT_ALL:
                for (unsigned int i = 0; i < INDEX_QTY; i++)
                {
                    mRedirect_W[i] = lTo;
                }
                break;

            case What::WHAT_STDERR:
                mRedirect_W[INDEX_STDERR] = lTo;
                mRedirect_W[INDEX_STDOUT] = INVALID_HANDLE_VALUE;
                break;

            case What::WHAT_STDOUT:
                mRedirect_W[INDEX_STDERR] = INVALID_HANDLE_VALUE;
                mRedirect_W[INDEX_STDOUT] = lTo;
                break;

            default: assert(false);
            }
        }

        void Redirection::Destruct()
        {
            for (unsigned int i = 0; i < FILE_QTY; i++)
            {
                if (HANDLE_INVALID != mFileNos[i])
                {
                    auto lRet = _close(mFileNos[i]);
                    assert(0 == lRet);
                }

                if (INVALID_HANDLE_VALUE != mFiles[i])
                {
                    auto lRet = CloseHandle(mFiles[i]);
                    assert(lRet);
                }
            }
        }

        int Redirection::OpenFileIfNeeded()
        {
            if (INVALID_HANDLE_VALUE == mFiles[FILE_FILE])
            {
                char lFile[NAME_LENGTH];

                sprintf_s(lFile, "%u.txt", OS::GetProcessId());

                char lPath[PATH_LENGTH];

                sFolder.GetPath(lFile, lPath, sizeof(lPath));

                mRedirect_W[FILE_FILE] = CreateFile(lPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);

                OpenOSFile(FILE_FILE);
            }

            return mFileNos[FILE_FILE];
        }

        int Redirection::OpenNullIfNeeded()
        {
            if (INVALID_HANDLE_VALUE == mFiles[FILE_NULL])
            {
                mFiles[FILE_NULL] = CreateFile(NULL_DEV, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

                OpenOSFile(FILE_NULL);
            }

            return mFileNos[FILE_NULL];
        }

        void Redirection::Redirect_OSDep() { RedirectIfNeeded(mRedirect_W); }
        void Redirection::Restore_OSDep () { RedirectIfNeeded(mRestore_W ); }

        void Redirection::Save_OSDep(What aWhat)
        {
            switch (aWhat)
            {
            case What::WHAT_ALL:
                mRestore_W[INDEX_STDERR] = GetStdHandle(STD_ERROR_HANDLE);
                mRestore_W[INDEX_STDOUT] = GetStdHandle(STD_OUTPUT_HANDLE);
                break;

            case What::WHAT_STDERR:
                mRestore_W[INDEX_STDERR] = GetStdHandle(STD_ERROR_HANDLE);
                mRestore_W[INDEX_STDOUT] = INVALID_HANDLE_VALUE;
                break;

            case What::WHAT_STDOUT:
                mRestore_W[INDEX_STDERR] = INVALID_HANDLE_VALUE;
                mRestore_W[INDEX_STDOUT] = GetStdHandle(STD_OUTPUT_HANDLE);
                break;

            default: assert(false);
            }
        }

        void Redirection::OpenOSFile(unsigned int aFile)
        {
            assert(FILE_QTY > aFile);

            if (INVALID_HANDLE_VALUE != mFiles[aFile])
            {
                mFileNos[aFile] = _open_osfhandle(reinterpret_cast<intptr_t>(mFiles[aFile]), O_WRONLY | O_TEXT);
                assert(HANDLE_INVALID != mFileNos[aFile]);
            }
        }
    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void RedirectIfNeeded(HANDLE aRedirect[INDEX_QTY])
{
    if (INVALID_HANDLE_VALUE != aRedirect[INDEX_STDERR]) { SetStdHandle(STD_ERROR_HANDLE , aRedirect[INDEX_STDERR]); }
    if (INVALID_HANDLE_VALUE != aRedirect[INDEX_STDOUT]) { SetStdHandle(STD_OUTPUT_HANDLE, aRedirect[INDEX_STDOUT]); }
}
