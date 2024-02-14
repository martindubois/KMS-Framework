
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Console_Redirection.cpp

#include "Component.h"

// ===== C ==================================================================
#include <io.h>

// ===== Includes ===========================================================
#include <KMS/Console/Redirection.h>
#include <KMS/OS.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define HANDLE_INVALID (-1)
#define HANDLE_STDERR  (2)
#define HANDLE_STDOUT  (1)

#define INDEX_STDERR (0)
#define INDEX_STDOUT (1)

#define INDEX_QTY (2)

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void RedirectIfNeeded(const int aRedirect[2]);

namespace KMS
{
    namespace Console
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const File::Folder Redirection::FOLDER_DEFAULT = File::Folder::CURRENT;

        File::Folder Redirection::sFolder = FOLDER_DEFAULT;

        Redirection::Redirection(What aWhat, To aTo)
        {
            Construct_Begin();

            Save(aWhat);

            int lTo;

            switch (aTo)
            {
            case To::TO_FILE: lTo = OpenFileIfNeeded(); break;
            case To::TO_NULL: lTo = OpenNullIfNeeded(); break;

            default: assert(false);
            }

            switch (aWhat)
            {
            case What::WHAT_ALL:
                for (unsigned int i = 0; i < INDEX_QTY; i++)
                {
                    mRedirect[i] = lTo;
                }
                break;

            case What::WHAT_STDERR:
                mRedirect[INDEX_STDERR] = lTo;
                mRedirect[INDEX_STDOUT] = HANDLE_INVALID;
                break;

            case What::WHAT_STDOUT:
                mRedirect[0] = HANDLE_INVALID;
                mRedirect[1] = lTo;
                break;

            default: assert(false);
            }

            Construct_End(aWhat, aTo);

            Redirect();
        }

        Redirection::~Redirection()
        {
            Restore();
            Destruct();
        }

        void Redirection::Redirect() { Redirect_OSDep(); RedirectIfNeeded(mRedirect); }
        void Redirection::Restore () { Restore_OSDep (); RedirectIfNeeded(mRestore ); }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Redirection::Save(What aWhat)
        {
            switch (aWhat)
            {
            case What::WHAT_ALL:
                mRestore[INDEX_STDERR] = _dup(HANDLE_STDERR);
                mRestore[INDEX_STDOUT] = _dup(HANDLE_STDOUT);
                break;

            case What::WHAT_STDERR:
                mRestore[INDEX_STDERR] = _dup(HANDLE_STDERR);
                mRestore[INDEX_STDOUT] = HANDLE_INVALID;
                break;

            case What::WHAT_STDOUT:
                mRestore[INDEX_STDERR] = HANDLE_INVALID;
                mRestore[INDEX_STDOUT] = _dup(HANDLE_STDOUT);
                break;

            default: assert(false);
            }

            Save_OSDep(aWhat);
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void RedirectIfNeeded(const int aRedirect[INDEX_QTY])
{
    int lRet;

    if (HANDLE_INVALID != aRedirect[INDEX_STDERR])
    {
        fflush(stderr);
        lRet = _dup2(aRedirect[INDEX_STDERR], HANDLE_STDERR);
        assert(0 == lRet);
    }

    if (HANDLE_INVALID != aRedirect[INDEX_STDOUT])
    {
        fflush(stdout);
        lRet = _dup2(aRedirect[INDEX_STDOUT], HANDLE_STDOUT);
        assert(0 == lRet);
    }
}
