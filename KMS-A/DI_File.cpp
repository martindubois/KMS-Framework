
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

        File::File() : mInternal(nullptr) {}

        File::File(FILE* aFile, const char* aIn) : String_Expand(aIn), mInternal(aFile), mMode("rb") {}

        File::operator FILE* () { return mInternal; }

        bool File::IsOpen() const { return nullptr != mInternal; }

        void File::SetMode(const char* aMode) { mMode = aMode; }

        void File::Close()
        {
            if (   (nullptr != mInternal)
                && (stderr != mInternal)
                && (stdin  != mInternal)
                && (stdout != mInternal))
            {
                auto lRet = fclose(mInternal);
                assert(0 == lRet);
            }

            mInternal = nullptr;
        }

        // ===== Object =====================================================
        File::~File() { Close(); }

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Object =====================================================

        void File::Send_OnChanged(void* aData)
        {
            Close();

            assert(nullptr == mInternal);

            auto lIn = Get();

            if      (0 == strcmp("stderr", lIn)) { mInternal = stderr; }
            else if (0 == strcmp("stdin" , lIn)) { mInternal = stdin; }
            else if (0 == strcmp("stdout", lIn)) { mInternal = stdout; }
            else
            {
                auto lRet = fopen_s(&mInternal, lIn, mMode);

                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open \"%s\"", lIn);
                KMS_EXCEPTION_ASSERT(0 == lRet, DI_OPEN_FAILED, lMsg, lRet);
            }

            assert(nullptr != mInternal);

            Object::Send_OnChanged(aData);
        }

    }
}
