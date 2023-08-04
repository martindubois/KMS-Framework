
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Boolean.cpp

// TEST COVERAGE  2023-07-28  KMS - Martin Dubois. P. Eng.

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

        Boolean::Boolean(bool aIn) : mInternal(aIn) {}

        void Boolean::operator = (bool aIn) { mInternal = aIn; }

        Boolean::operator bool() const { return mInternal; }

        bool Boolean::Get() const { return mInternal; }

        void Boolean::Set(bool aIn) { mInternal = aIn; }

        // ===== Value ======================================================

        unsigned int Boolean::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            KMS_EXCEPTION_ASSERT(7 <= aOutSize_byte, DI_OUTPUT_TOO_SHORT, "The output buffer is too short", aOutSize_byte);

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

        void Boolean::Set(const char* aIn)
        {
            auto lIn = Convert::ToBool(aIn);
            if (mInternal != lIn)
            {
                mInternal = lIn;

                Send_OnChanged(const_cast<char*>(aIn));
            }
        }

        // ===== Object =====================================================

        Boolean::~Boolean() {}

        bool Boolean::Clear()
        {
            auto lResult = !mInternal;

            mInternal = false;

            return lResult;
        }

    }
}
