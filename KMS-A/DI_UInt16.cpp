
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_UInt16.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/UInt16.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Object* UInt16::Create() { return new UInt16(); }

        UInt16::UInt16(uint16_t aIn) : mInternal(aIn), mRadix(KMS::Radix::DECIMAL) {}

        UInt16::operator uint16_t () const { return mInternal; }

        uint16_t UInt16::Get() const { return mInternal; }

        // ===== Value ======================================================

        unsigned int UInt16::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            switch (mRadix)
            {
            case KMS::Radix::DECIMAL    : lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u", mInternal); break;
            case KMS::Radix::HEXADECIMAL: lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%x", mInternal); break;

            default: assert(false);
            }

            return lResult_byte;
        }

        void UInt16::Set(const char* aIn)
        {
            assert(NULL != aIn);

            int lRet;

            unsigned int lIn;

            switch (mRadix)
            {
            case KMS::Radix::DECIMAL    : lRet = sscanf_s(aIn, "%u", &lIn); break;
            case KMS::Radix::HEXADECIMAL: lRet = sscanf_s(aIn, "%x", &lIn); break;

            default: assert(false);
            }

            KMS_EXCEPTION_ASSERT(0xffff >= lIn, CONFIG_VALUE, "The value is above the limit for a 16 bits value");

            mInternal = lIn;

            assert(1 == lRet);
        }

        // ===== Object =====================================================

        UInt16::~UInt16() {};

    }
}
