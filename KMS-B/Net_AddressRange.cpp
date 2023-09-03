
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_AddressRange.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/SafeAPI.h>

#include <KMS/Net/AddressRange.h>

KMS_RESULT_STATIC(RESULT_INVALID_ADDRESS_RANGE);

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        AddressRange::AddressRange() { Construct(); }

        void AddressRange::operator = (const Address& aA)
        {
            mType = aA.GetType();

            memcpy(mData, aA.GetBinary(), GetSize());

            ResetMask();
        }

        bool AddressRange::operator == (const Address& aA) const
        {
            bool lResult = false;

            if (mType == aA.GetType())
            {
                auto lA = reinterpret_cast<const uint8_t*>(aA.GetBinary());
                auto lSize_byte = GetSize();

                lResult = true;

                for (unsigned int i = 0; i < lSize_byte; i++)
                {
                    if ((mData[i] & mMask[i]) != (lA[i] & mMask[i]))
                    {
                        lResult = false;
                        break;
                    }
                }
            }

            return lResult;
        }

        void AddressRange::SetMask(unsigned int aBits)
        {
            auto lBits = aBits;

            for (unsigned int i = 0; i < sizeof(mMask) / sizeof(mMask[1]); i++)
            {
                if (8 < lBits)
                {
                    mMask[i] = 0xff;
                    lBits -= 8;
                }
                else
                {
                    static const uint8_t MASKS[8] = { 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe };

                    mMask[i] = MASKS[lBits];
                    lBits = 0;
                }
            }

            KMS_EXCEPTION_ASSERT(0 >= lBits, RESULT_INVALID_ADDRESS_RANGE, "Invalid mask length", aBits);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void AddressRange::Construct()
        {
            mType = Address::Type::UNKNOWN;

            memset(&mData, 0, sizeof(mData));

            ResetMask();
        }

        unsigned int AddressRange::GetSize() const
        {
            unsigned int lResult_byte = 0;

            switch (mType)
            {
            case Address::Type::IPv4: lResult_byte = 4; break;
            case Address::Type::IPv6: lResult_byte = 8; break;

            default: assert(false);
            }

            return lResult_byte;
        }

        void AddressRange::ResetMask() { memset(&mMask, 0xff, sizeof(mMask)); }

    }
}
