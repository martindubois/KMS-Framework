
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_UInt32.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/UInt32.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        UInt32::UInt32(uint32_t aIn, const MetaData* aMD) : Value(aMD), mInternal(aIn), mRadix(KMS::Radix::DECIMAL) {}

        void UInt32::operator = (uint32_t aIn) { mInternal = aIn; }

        UInt32::operator uint32_t () const { return mInternal; }

        // ===== Value ======================================================

        unsigned int UInt32::Get(char* aOut, unsigned int aOutSize_byte) const
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

        void UInt32::Set(const char* aIn)
        {
            assert(NULL != aIn);

            int lRet;

            switch (mRadix)
            {
            case KMS::Radix::DECIMAL    : lRet = sscanf_s(aIn, "%u", &mInternal); break;
            case KMS::Radix::HEXADECIMAL: lRet = sscanf_s(aIn, "%x", &mInternal); break;

            default: assert(false);
            }

            assert(1 == lRet);
        }

        // ===== Object =====================================================

        UInt32::~UInt32() {};

    }
}
