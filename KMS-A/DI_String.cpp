
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

        String::String(const char* aIn, const MetaData* aMD) : Value(aMD), mInternal(aIn) {}

        // ===== Value ======================================================

        unsigned int String::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            unsigned int lResult_byte = static_cast<unsigned int>(mInternal.size());
            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, OUTPUT_TOO_SHORT, "The output buffer is too short");

            strcpy_s(aOut SizeInfoV(aOutSize_byte), mInternal.c_str());

            return lResult_byte;
        }

        void String::Set(const char* aIn) { mInternal = aIn; }

        // ===== Object =====================================================

        String::~String() {};

    }
}
