
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
#include <KMS/Convert.h>

#include <KMS/DI/GUID.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

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

            *mPtr = Convert::ToGUID(aIn);
        }

        bool GUID_Ptr::Set_Try(const char* aIn)
        {
            assert(nullptr != mPtr);

            return Convert::ToGUID_Try(aIn, mPtr);
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
