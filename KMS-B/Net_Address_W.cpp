
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Address_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <WS2tcpip.h>

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
                lB = &mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b1;
                sprintf_s(lName, "%u.%u.%u.%u", lB[0], lB[1], lB[2], lB[3]);
                break;

            case AF_INET6:
                lB = reinterpret_cast<uint8_t*>(&mAddress.mIPv6.sin6_addr.u.Byte);
                sprintf_s(lName, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", lB[0], lB[1], lB[2], lB[3], lB[4], lB[5], lB[6], lB[7]);
                break;

            default: assert(false);
            }

            mName = lName;
        }

        void Address::SetIPv4_OSDep(const unsigned int* aA)
        {
            assert(nullptr != aA);

            mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b1 = aA[0];
            mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b2 = aA[1];
            mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b3 = aA[2];
            mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b4 = aA[3];
        }

    }
}
