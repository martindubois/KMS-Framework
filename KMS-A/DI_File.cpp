
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_File.cpp

// TEST COVERAGE  2023-08-30  KMS - Martin Dubois. P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/File.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* DI::File::DEFAULT_MODE  = "rb";
        const char* DI::File::DEFAULT_VALUE = "";

        DI::Object* DI::File::Create() { return new File; }

        File_Ptr::File_Ptr(FILE** aPtr, const char* aIn) : String_Expand(aIn), mMode(DEFAULT_MODE), mPtr(aPtr)
        {
            assert(nullptr != aPtr);
        }

        File::File() : File_Ptr(&mInternal), mInternal(nullptr) {}

        File::File(FILE* aFile, const char* aIn) : File_Ptr(&mInternal, aIn), mInternal(aFile) {}

        File_Ptr::operator FILE* ()
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        bool File_Ptr::IsOpen() const
        {
            assert(nullptr != mPtr);

            return nullptr != *mPtr;
        }

        void File_Ptr::SetMode(const char* aMode)
        {
            assert(nullptr != aMode);

            mMode = aMode;
        }

        void File_Ptr::Close()
        {
            assert(nullptr != mPtr);

            auto lFile = *mPtr;

            if (   (nullptr != lFile)
                && (stderr  != lFile)
                && (stdin   != lFile)
                && (stdout  != lFile))
            {
                auto lRet = fclose(lFile);
                assert(0 == lRet);
            }

            *mPtr = nullptr;
        }

        // ===== Object =====================================================

        File_Ptr::~File_Ptr() { Close(); }

        bool File_Ptr::Clear()
        {
            Close();

            return String_Expand::Clear();
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Object =====================================================

        void File_Ptr::Send_OnChanged(void* aData)
        {
            assert(nullptr != mPtr);

            Close();

            assert(nullptr == *mPtr);

            auto lIn = Get();

            if      (0 == strcmp("stderr", lIn)) { *mPtr = stderr; }
            else if (0 == strcmp("stdin" , lIn)) { *mPtr = stdin; }
            else if (0 == strcmp("stdout", lIn)) { *mPtr = stdout; }
            else
            {
                auto lRet = fopen_s(mPtr, lIn, mMode);
                if (0 != lRet)
                {
                    char lMsg[64 + PATH_LENGTH];
                    sprintf_s(lMsg, "Cannot open \"%s\"", lIn);
                    KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, lRet);
                }
            }

            assert(nullptr != *mPtr);

            Object::Send_OnChanged(aData);
        }

    }
}
