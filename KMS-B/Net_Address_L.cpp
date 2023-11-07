
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Address_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Address.h>

namespace KMS
{
    namespace Net
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Address::UpdateName()
        {
            const uint8_t* lB;

            char lName[32];

            switch (mAddress.mBase.sa_family)
            {
            case AF_INET:
                lB = reinterpret_cast<const uint8_t*>(&mAddress.mIPv4.sin_addr.s_addr);
                sprintf_s(lName, "%u.%u.%u.%u", lB[0], lB[1], lB[2], lB[3]);
                break;

            case AF_INET6:
                lB = reinterpret_cast<const uint8_t*>(&mAddress.mIPv6.sin6_addr.s6_addr);
                sprintf_s(lName, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", lB[0], lB[1], lB[2], lB[3], lB[4], lB[5], lB[6], lB[7]);
                break;

            default: assert(false);
            }

            mName = lName;
        }

        void Address::SetIPv4_OSDep(const unsigned int* aA)
        {
            assert(nullptr != aA);

            auto lA = reinterpret_cast<uint8_t*>(&mAddress.mIPv4.sin_addr.s_addr);

            for (unsigned int i = 0; i < 4; i++)
            {
                lA[i] = aA[i];
            }
        }

    }
}
