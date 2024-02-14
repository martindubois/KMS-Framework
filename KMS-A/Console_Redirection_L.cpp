
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Console_Redirection_L.cpp

#include "Component.h"

// ===== C ==================================================================
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

#define NULL_DEV "/dev/null"

namespace KMS
{
    namespace Console
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Redirection::Construct_Begin()
        {
            for (unsigned int i = 0; i < FILE_QTY; i++)
            {
                mFiles[i] = nullptr;
            }
        }

        void Redirection::Construct_End(What, To) {}

        void Redirection::Destruct()
        {
            for (unsigned int i = 0; i < FILE_QTY; i++)
            {
                if (nullptr != mFiles[i])
                {
                    auto lRet = fclose(mFiles[i]);
                    assert(0 == lRet);
                }
            }
        }

        int Redirection::OpenFileIfNeeded()
        {
            if (nullptr == mFiles[FILE_FILE])
            {
                char lFile[NAME_LENGTH];

                sprintf_s(lFile, "%u.txt", OS::GetProcessId());

                char lPath[PATH_LENGTH];

                sFolder.GetPath(lFile, lPath, sizeof(lPath));

                auto lRet = fopen_s(mFiles + FILE_FILE, lPath, "wb+");
                assert(0 == lRet);
            }

            return (nullptr != mFiles[FILE_FILE]) ? _fileno(mFiles[FILE_FILE]) : HANDLE_INVALID;
        }

        int Redirection::OpenNullIfNeeded()
        {
            if (nullptr == mFiles[FILE_NULL])
            {
                auto lRet = fopen_s(mFiles + FILE_NULL, NULL_DEV, "wb");
                assert(0 == lRet);
            }

            return (nullptr != mFiles[FILE_NULL]) ? _fileno(mFiles[FILE_NULL]) : HANDLE_INVALID;
        }

        void Redirection::Redirect_OSDep() {}
        void Redirection::Restore_OSDep () {}

        void Redirection::Save_OSDep(What) {}

    }
}
