
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_String.cpp

// TEST COVERAGE  2023-08-01  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/String.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        DI::Object* String::Create() { return new String; }

        String::String(const char* aIn) : mInternal(aIn) {}

        String_Ptr::String_Ptr(std::string* aString) : mInternal(aString) {}

        void String_Base::operator = (const char* aIn) { *Internal_Get() = aIn; }

        bool String_Base::operator == (const char* aIn) const { return *Internal_Get() == aIn; }

        String_Base::operator const char* () const { return Internal_Get()->c_str(); }

        const char* String_Base::Get() const { return Internal_Get()->c_str(); }

        unsigned int String_Base::GetLength() const { return static_cast<unsigned int>(Internal_Get()->size()); }

        const std::string& String_Base::GetString() const { return *Internal_Get(); }

        // ===== String_Base ================================================

        String_Base::~String_Base() {}

        // ===== Value ======================================================

        unsigned int String_Base::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);

            auto lInternal = Internal_Get();
            assert(nullptr != lInternal);

            auto lResult_byte = static_cast<unsigned int>(lInternal->size());
            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, DI_OUTPUT_TOO_SHORT, "The output buffer is too short", lResult_byte);

            strcpy_s(aOut SizeInfoV(aOutSize_byte), lInternal->c_str());

            return lResult_byte;
        }

        void String_Base::Set(const char* aIn)
        {
            assert(nullptr != aIn);

            auto lInternal = Internal_Get();
            assert(nullptr != lInternal);

            if (*lInternal != aIn)
            {
                *lInternal = aIn;

                Send_OnChanged(const_cast<char*>(aIn));
            }
        }

        // ===== Object =====================================================

        String    ::~String    () {}
        String_Ptr::~String_Ptr() {}

        bool String_Base::Clear()
        {
            auto lInternal = Internal_Get();
            assert(nullptr != lInternal);

            auto lResult = !lInternal->empty();

            lInternal->clear();

            return lResult;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        String_Base::String_Base() {}

        // ===== String_Base ================================================

        const std::string* String    ::Internal_Get() const { return &mInternal; }
              std::string* String    ::Internal_Get()       { return &mInternal; }
        const std::string* String_Ptr::Internal_Get() const { return  mInternal; }
              std::string* String_Ptr::Internal_Get()       { return  mInternal; }

    }
}
