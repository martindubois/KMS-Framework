
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Network_AddressRange.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/SafeAPI.h>

#include <KMS/Network/AddressRange.h>

namespace KMS
{
    namespace Network
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        AddressRange::AddressRange(const char* aAR)
        {
            Construct();

            Set(aAR);
        }

        const AddressRange& AddressRange::operator = (const Address& aA)
        {
            mType = aA.GetType();

            memcpy(mData, aA.GetBinary(), GetSize());

            ResetMask();

            return *this;
        }

        bool AddressRange::operator == (const Address& aA) const
        {
            bool lResult = false;

            if (mType == aA.GetType())
            {
                const uint8_t* lA = reinterpret_cast<const uint8_t*>(aA.GetBinary());
                unsigned int lSize_byte = GetSize();

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

        void AddressRange::Set(const char* aAR)
        {
            assert(NULL != aAR);

            char lA[LINE_LENGTH];
            unsigned int lM;

            if (2 == sscanf_s(aAR, "%[^/]/%u", lA SizeInfo(lA), &lM))
            {
                *this = Address(lA);
                SetMask(lM);
                return;
            }

            *this = Address(aAR);
        }

        void AddressRange::SetMask(unsigned int aBits)
        {
            unsigned int lBits = aBits;

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

            if (0 < lBits)
            {
                KMS_EXCEPTION_WITH_INFO(NETWORK_ADDRESS_RANGE, "Invalid mask length", aBits);
            }
        }

    }
}
