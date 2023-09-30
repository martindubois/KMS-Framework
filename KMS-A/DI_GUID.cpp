
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_GUID.cpp

// TEST COVERAGE  2023-08-30  KMS - Martin Dubois. P. Eng.

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/DI/GUID.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static ::GUID ToGUID(const char* aIn);

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const ::GUID GUID::DEFAULT_VALUE;

        Object* GUID::Create() { return new GUID; }

        GUID_Ptr::GUID_Ptr(::GUID* aPtr) : mPtr(aPtr)
        {
            assert(nullptr != mPtr);
        }

        GUID::GUID(const ::GUID& aG) : GUID_Ptr(&mInternal), mInternal(aG) {}

        void GUID_Ptr::operator = (const ::_GUID& aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = aIn;
        }

        GUID_Ptr::operator const ::GUID& () const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        bool GUID_Ptr::IsValid() const
        {
            assert(nullptr != mPtr);

            auto lPtr = reinterpret_cast<const uint8_t*>(mPtr);

            for (unsigned int i = 0; i < sizeof(::GUID); i++)
            {
                if (0 != lPtr[i])
                {
                    return true;
                }
            }

            return false;
        }

        // ===== Value ======================================================

        unsigned int KMS::DI::GUID_Ptr::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);
            assert(40 <= aOutSize_byte);

            assert(nullptr != mPtr);

            const auto lD4 = mPtr->Data4;

            return sprintf_s(aOut SizeInfoV(aOutSize_byte),
                "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
                mPtr->Data1, mPtr->Data2, mPtr->Data3, mPtr->Data4[0],
                lD4[1], lD4[2], lD4[3], lD4[4], lD4[5], lD4[6], lD4[7]);
        }

        void GUID_Ptr::Set(const char* aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = ToGUID(aIn);
        }

        // ===== Object =====================================================

        bool GUID_Ptr::Clear()
        {
            assert(nullptr != mPtr);

            auto lResult = 0 != memcmp(&DEFAULT_VALUE, mPtr, sizeof(DEFAULT_VALUE));

            *mPtr = DEFAULT_VALUE;

            return lResult;
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

GUID ToGUID(const char* aIn)
{
    assert(nullptr != aIn);

    GUID lResult;

    memset(&lResult, 0, sizeof(lResult));

    // {00000000-0000-0000-0000-000000000000}
    // 01234567890123456789012345678901234567
    //           1         2         3

    uint32_t lValue = 0;

    for (unsigned int i = 1; i <= 36; i++)
    {
        lValue <<= 8;

        switch (aIn[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': lValue |= aIn[i] - '0'; break;

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f': lValue |= aIn[i] - 'a' + 10; break;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F': lValue |= aIn[i] - 'A' + 10; break;

        case '-': break;

        default: KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid GUID format", aIn);
        }

        switch (i)
        {
        case  8: lResult.Data1 = lValue; lValue = 0; break;
        case 13: lResult.Data2 = lValue; lValue = 0; break;
        case 18: lResult.Data3 = lValue; lValue = 0; break;

        case 21:
        case 23: lResult.Data4[(i - 21) / 2] = lValue; lValue = 0; break;

        case 26:
        case 28:
        case 30:
        case 32:
        case 34:
        case 36: lResult.Data4[2 + (i - 26) / 2] = lValue; lValue = 0; break;
        }
    }

    return lResult;
}
