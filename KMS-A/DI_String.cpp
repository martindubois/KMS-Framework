
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_String.cpp

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

        void String::operator = (const char* aIn) { mInternal = aIn; }

        bool String::operator == (const char* aIn) const { return mInternal == aIn; }

        String::operator const char* () const { return mInternal.c_str(); }

        const char* String::Get() const { return mInternal.c_str(); }

        unsigned int String::GetLength() const { return static_cast<unsigned int>(mInternal.size()); }

        // ===== Value ======================================================

        unsigned int String::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            unsigned int lResult_byte = static_cast<unsigned int>(mInternal.size());
            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, DI_OUTPUT_TOO_SHORT, "The output buffer is too short", lResult_byte);

            strcpy_s(aOut SizeInfoV(aOutSize_byte), mInternal.c_str());

            return lResult_byte;
        }

        void String::Set(const char* aIn) { mInternal = aIn; }

        // ===== Object =====================================================

        String::~String() {};

        void String::Clear() { mInternal.clear(); }

    }
}
