
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Boolean.cpp

// TEST COVERAGE  2023-08-30  KMS - Martin Dubois. P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

#include <KMS/DI/Boolean.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const bool DEFAULT_VALUE = false;

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const bool Boolean_Ptr::DEFAULT_VALUE = false;

        DI::Object* Boolean::Create() { return new Boolean; }

        Boolean_Ptr::Boolean_Ptr(bool* aPtr) : mPtr(aPtr)
        {
            assert(nullptr != aPtr);
        }

        Boolean::Boolean(bool aIn) : Boolean_Ptr(&mInternal), mInternal(aIn) {}

        void Boolean_Ptr::operator = (bool aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = aIn;
        }

        Boolean_Ptr::operator bool() const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        bool Boolean_Ptr::Get() const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        void Boolean_Ptr::Set(bool aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = aIn;
        }

        // ===== Value ======================================================

        unsigned int Boolean_Ptr::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != mPtr);

            KMS_EXCEPTION_ASSERT(7 <= aOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aOutSize_byte);

            unsigned int lResult_byte;

            if (*mPtr)
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

        void Boolean_Ptr::Set(const char* aIn)
        {
            assert(nullptr != mPtr);

            auto lIn = Convert::ToBool(aIn);
            if (*mPtr != lIn)
            {
                *mPtr = lIn;

                Send_OnChanged(const_cast<char*>(aIn));
            }
        }

        bool Boolean_Ptr::Set_Try(const char* aIn)
        {
            assert(nullptr != mPtr);

            bool lIn;

            auto lResult = Convert::ToBool_Try(aIn, &lIn);
            if (lResult && (*mPtr != lIn))
            {
                *mPtr = lIn;

                Send_OnChanged(const_cast<char*>(aIn));
            }

            return lResult;
        }

        // ===== Object =====================================================

        bool Boolean_Ptr::Clear()
        {
            assert(nullptr != mPtr);

            auto lResult = *mPtr != DEFAULT_VALUE;

            *mPtr = DEFAULT_VALUE;

            return lResult;
        }

    }
}
