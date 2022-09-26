
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Boolean.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

#include <KMS/DI/Boolean.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Boolean::Boolean(bool aIn, const MetaData* aMD) : Value(aMD), mInternal(aIn) {}

        void Boolean::operator = (bool aIn) { mInternal = aIn; }

        Boolean::operator bool() const { return mInternal; }

        // ===== Value ======================================================

        unsigned int Boolean::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            KMS_EXCEPTION_ASSERT(7 <= aOutSize_byte, OUTPUT_TOO_SHORT, "The output buffer is too short");

            unsigned int lResult_byte;

            if (mInternal)
            {
                strcpy_s(aOut SizeInfoV(aOutSize_byte), "true");
                lResult_byte = 4;
            }
            else
            {
                strcpy_s(aOut SizeInfoV(aOutSize_byte), "false");
                lResult_byte = 5;
            }

            return lResult_byte;
        }

        void Boolean::Set(const char* aIn) { mInternal = Convert::ToBool(aIn); }

        // ===== Object =====================================================

        Boolean::~Boolean() {}

    }
}
