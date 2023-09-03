
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_String.cpp

// TEST COVERAGE  2023-08-30  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/String.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static std::string DEFAULT_VALUE;

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        DI::Object* String::Create() { return new String; }

        String_Ptr::String_Ptr(std::string* aPtr) : mPtr(aPtr) {}

        String::String(const char* aIn) : String_Ptr(&mInternal), mInternal(aIn) {}

        void String_Ptr::operator = (const char* aIn)
        {
            assert(nullptr != aIn);

            assert(nullptr != mPtr);

            *mPtr = aIn;
        }

        String_Ptr::operator const char* () const
        {
            assert(nullptr != mPtr);

            return mPtr->c_str();
        }

        bool String_Ptr::operator == (const char* aIn) const
        {
            assert(nullptr != aIn);

            assert(nullptr != mPtr);

            return *mPtr == aIn;
        }

        const char* String_Ptr::Get() const
        {
            assert(nullptr != mPtr);

            return mPtr->c_str();
        }

        unsigned int String_Ptr::GetLength() const
        {
            assert(nullptr != mPtr);

            return static_cast<unsigned int>(mPtr->size());
        }

        const std::string& String_Ptr::GetString() const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        // ===== Value ======================================================

        unsigned int String_Ptr::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);

            assert(nullptr != mPtr);

            auto lResult_byte = static_cast<unsigned int>(mPtr->size());
            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", lResult_byte);

            strcpy_s(aOut SizeInfoV(aOutSize_byte), mPtr->c_str());

            return lResult_byte;
        }

        void String_Ptr::Set(const char* aIn)
        {
            assert(nullptr != aIn);

            assert(nullptr != mPtr);

            if (*mPtr != aIn)
            {
                *mPtr = aIn;

                Send_OnChanged(const_cast<char*>(aIn));
            }
        }

        // ===== Object =====================================================

        bool String_Ptr::Clear()
        {
            assert(nullptr != mPtr);

            auto lResult = *mPtr != DEFAULT_VALUE;

            *mPtr = DEFAULT_VALUE;

            return lResult;
        }

    }
}
