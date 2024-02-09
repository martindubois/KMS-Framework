
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Address.cpp

// TEST_COVERAGE 2023-10-12 Martin Dubois

// NOT TESTED  IPv6

#include "Component.h"

#ifdef _KMS_LINUX_
    // ===== C ==============================================================
    #include <netdb.h>
#endif

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <WS2tcpip.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Net/Address.h>

KMS_RESULT_STATIC(RESULT_ADDRESS_RESOLUTION_FAILED);
KMS_RESULT_STATIC(RESULT_INVALID_ADDRESS);
KMS_RESULT_STATIC(RESULT_INVALID_PORT);

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Address::Address() { Clear(); }

        void Address::Clear()
        {
            memset(&mAddress, 0, sizeof(mAddress));
        }

        const char* Address::GetName() const { return mName.c_str(); }

        uint16_t Address::GetPortNumber() const
        {
            uint16_t lResult = 0;

            switch (mAddress.mBase.sa_family)
            {
            case AF_INET : lResult = ntohs(mAddress.mIPv4.sin_port ); break;
            case AF_INET6: lResult = ntohs(mAddress.mIPv6.sin6_port); break;
            }

            return lResult;
        }

        Address::Type Address::GetType() const
        {
            auto lResult = Type::UNKNOWN;

            switch (mAddress.mBase.sa_family)
            {
            case AF_INET : lResult = Type::IPv4; break;
            case AF_INET6: lResult = Type::IPv6; break;
            }

            return lResult;
        }

        void Address::SetPortNumber(uint16_t aP)
        {
            switch (mAddress.mBase.sa_family)
            {
            case AF_INET : mAddress.mIPv4.sin_port  = htons(aP); break;
            case AF_INET6: mAddress.mIPv6.sin6_port = htons(aP); break;

            default: assert(false);
            }
        }

        void Address::SetType(Type aT)
        {
            switch (aT)
            {
            case Type::IPv4: mAddress.mBase.sa_family = AF_INET ; break;
            case Type::IPv6: mAddress.mBase.sa_family = AF_INET6; break;

            default: Clear();
            }
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const unsigned int Address::INTERNAL_SIZE_byte = sizeof(Address::mAddress);

        Address::operator sockaddr* () { return &mAddress.mBase; }

        // Return nullptr it the address type is UNKNOWN.
        const void* Address::GetBinary() const
        {
            const void* lResult = nullptr;

            switch (mAddress.mBase.sa_family)
            {
            case AF_INET : lResult = &mAddress.mIPv4.sin_addr ; break;
            case AF_INET6: lResult = &mAddress.mIPv6.sin6_addr; break;
            }

            return lResult;
        }

        int Address::GetInternalFamily() const { return mAddress.mBase.sa_family; }

        int Address::GetInternalSize() const
        {
            int lResult_byte = 0;

            switch (mAddress.mBase.sa_family)
            {
            case AF_INET : lResult_byte = sizeof(mAddress.mIPv4); break;
            case AF_INET6: lResult_byte = sizeof(mAddress.mIPv6); break;
            }

            return lResult_byte;
        }

        void Address::SetAddress(const char* aA)
        {
            assert(nullptr != aA);

            unsigned int lB[4];

            if (4 == sscanf_s(aA, "%u.%u.%u.%u", lB + 0, lB + 1, lB + 2, lB + 3))
            {
                SetIPv4(lB);
                return;
            }

            SetName(aA);
        }

        void Address::SetInternal(const struct sockaddr* aAddr, unsigned int aAddrSize_byte)
        {
            assert(nullptr != aAddr);
            assert(0 < aAddrSize_byte);
            assert(sizeof(mAddress) >= aAddrSize_byte);

            memcpy(&mAddress.mBase, aAddr, aAddrSize_byte);

            SetInternalSize(aAddrSize_byte);
        }

        void Address::SetInternalSize(unsigned int aAddrSize_byte)
        {
            switch (aAddrSize_byte)
            {
            case sizeof(mAddress.mIPv4) : mAddress.mBase.sa_family = AF_INET ; break;
            case sizeof(mAddress.mIPv6) : mAddress.mBase.sa_family = AF_INET6; break;

            default: assert(false);
            }

            UpdateName();
        }

        void Address::SetPortNumber(unsigned int aP)
        {
            if (0xffff < aP)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid port number", aP);
                KMS_EXCEPTION(RESULT_INVALID_PORT, lMsg, "");
            }

            SetPortNumber(static_cast<uint16_t>(aP));
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Address::SetIPv4(const unsigned int* aA)
        {
            assert(nullptr != aA);

            for (unsigned int i = 0; i < 4; i++)
            {
                KMS_EXCEPTION_ASSERT(0xff >= aA[i], RESULT_INVALID_ADDRESS, "Invalid IPv4 address", aA[i]);
            }

            SetIPv4_OSDep(aA);

            mAddress.mIPv4.sin_family = AF_INET;

            UpdateName();
        }

        void Address::SetName(const char* aN)
        {
            assert(nullptr != aN);

            char lMsg[64 + NAME_LENGTH];

            addrinfo* lAddr;

            auto lRet = getaddrinfo(aN, NULL, NULL, &lAddr);
            if (0 != lRet)
            {
                sprintf_s(lMsg, "Cannot resolve the network address \"%s\"", aN);
                KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_ADDRESS_RESOLUTION_FAILED, lMsg, lRet);
            }

            assert(nullptr != lAddr);

            auto lCurrent = lAddr;

            while (nullptr != lCurrent)
            {
                if ((AF_INET == lCurrent->ai_family) || (AF_INET6 == lCurrent->ai_family))
                {
                    SetInternal(lAddr->ai_addr, static_cast<unsigned int>(lAddr->ai_addrlen));

                    freeaddrinfo(lAddr);

                    mName = aN;

                    return;
                }

                // NOT TESTED
                lCurrent = lCurrent->ai_next;
            }

            freeaddrinfo(lAddr);

            sprintf_s(lMsg, "Cannot resolve the network address \"%s\" (NOT TESTED)", aN);
            KMS_EXCEPTION(RESULT_ADDRESS_RESOLUTION_FAILED, lMsg, "");
        }

    }
}
