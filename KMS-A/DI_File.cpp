
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_File.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/File.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        File::File(FILE* aFile, const char* aIn) : String_Expand(aIn), mInternal(aFile), mMode("rb") {}

        File::operator FILE* () { return mInternal; }

        void File::SetMode(const char* aMode) { mMode = aMode; }

        void File::Close()
        {
            if (   (NULL   != mInternal)
                && (stderr != mInternal)
                && (stdin  != mInternal)
                && (stdout != mInternal))
            {
                int lRet = fclose(mInternal);
                assert(0 == lRet);
            }

            mInternal = NULL;
        }

        // ===== Value ======================================================

        void File::Set(const char* aIn)
        {
            String_Expand::Set(aIn);

            Close();

            assert(NULL == mInternal);

            if      (0 == strcmp("stderr", aIn)) { mInternal = stderr; }
            else if (0 == strcmp("stdin" , aIn)) { mInternal = stdin ; }
            else if (0 == strcmp("stdout", aIn)) { mInternal = stdout; }
            else
            {
                errno_t lRet = fopen_s(&mInternal, *this, mMode);
                KMS_EXCEPTION_ASSERT(0 == lRet, DI_OPEN_FAILED, "Cannot open file", aIn);
            }

            assert(NULL != mInternal);
        }

        // ===== Object =====================================================
        File::~File() { Close(); }

    }
}
