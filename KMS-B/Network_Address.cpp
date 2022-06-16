
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Network_Address.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <WS2tcpip.h>

// ===== Includes ===========================================================
#include <KMS/Network/Address.h>
#include <KMS/SafeAPI.h>

namespace KMS
{
    namespace Network
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Address::Address() { Clear(); }

        Address::Address(const char* aA)
        {
            Clear();

            Set(aA);
        }

        void Address::Clear()
        {
            mSize_byte = 0;
            mType = Type::UNKNOWN;

            memset(&mAddress, 0, sizeof(mAddress));
        }

        Address::Type Address::GetType() const { return mType; }

        void Address::SetPortNumber(uint16_t aP)
        {
            switch (mType)
            {
            case Type::IPv4: mAddress.mIPv4.sin_port  = htons(aP); break;
            case Type::IPv6: mAddress.mIPv6.sin6_port = htons(aP); break;

            default: assert(false);
            }
        }

        void Address::SetType(Type aT) { mType = aT; }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const unsigned int Address::INTERNAL_SIZE_byte = sizeof(Address::mAddress);

        Address::operator sockaddr* () { return &mAddress.mBase; }

        // Return NULL it the address type is UNKNOWN.
        const void* Address::GetBinary() const
        {
            const void* lResult = NULL;

            switch (mType)
            {
            case Type::IPv4: lResult = &mAddress.mIPv4.sin_addr; break;
            case Type::IPv6: lResult = &mAddress.mIPv6.sin6_addr; break;
            }

            return lResult;
        }

        int Address::GetInternalFamily() const { return mAddress.mBase.sa_family; }

        int Address::GetInternalSize() const { return mSize_byte; }

        void Address::SetInternal(const struct sockaddr* aAddr, unsigned int aAddrSize_byte)
        {
            assert(NULL != aAddr);
            assert(0 < aAddrSize_byte);
            assert(sizeof(mAddress) >= aAddrSize_byte);

            memcpy(&mAddress.mBase, aAddr, aAddrSize_byte);

            SetInternalSize(aAddrSize_byte);
        }

        void Address::SetInternalSize(unsigned int aAddrSize_byte)
        {
            switch (aAddrSize_byte)
            {
            case sizeof(mAddress.mIPv4) : mType = Type::IPv4; break;
            case sizeof(mAddress.mIPv6) : mType = Type::IPv6; break;

            default: assert(false);
            }

            mSize_byte = aAddrSize_byte;

            UpdateName();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Address::Set(const char* aA)
        {
            assert(NULL != aA);

            if (0 == _stricmp(aA, "IPv4")) { SetType(Type::IPv4); return; }
            if (0 == _stricmp(aA, "IPv6")) { SetType(Type::IPv6); return; }

            const char* lA = aA;

            if      (0 == _strnicmp(aA, "IPv4:", 5)) { SetType(Type::IPv4); lA += 5; }
            else if (0 == _strnicmp(aA, "IPv6:", 5)) { SetType(Type::IPv6); lA += 5; }

            char lN[LINE_LENGTH];
            unsigned int lP;

            if (2 == sscanf_s(lA, "%[^:]:%u", lN SizeInfo(lN), &lP)) { SetAddress(lN); SetPortNumber(lP); return; }

            SetAddress(lA);
        }

        void Address::SetAddress(const char* aA)
        {
            assert(NULL != aA);

            unsigned int lB[4];

            if (4 == sscanf_s(aA, "%u.%u.%u.%u", lB + 0, lB + 1, lB + 2, lB + 3)) { SetIPv4(lB); }

            SetName(aA);
        }

        void Address::SetIPv4(const unsigned int* aA)
        {
            assert(NULL != aA);

            for (unsigned int i = 0; i < 4; i++)
            {
                if (0xff < aA[i])
                {
                    KMS_EXCEPTION_WITH_INFO(NETWORK_ADDRESS, "Invalid IPv4 address", aA[i]);
                }
            }

            mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b1 = aA[0];
            mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b2 = aA[1];
            mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b3 = aA[2];
            mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b4 = aA[3];
            mAddress.mIPv4.sin_family = AF_INET;

            mSize_byte = sizeof(mAddress.mIPv4);
            mType      = Type::IPv4;

            UpdateName();
        }

        void Address::SetName(const char* aN)
        {
            assert(NULL != aN);

            addrinfo* lAddr;

            int lRet = getaddrinfo(aN, NULL, NULL, &lAddr);
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(NETWORK_ADDRESS, "Cannot resolve the network address", aN);
            }

            assert(NULL != lAddr);

            addrinfo* lCurrent = lAddr;

            while (NULL != lCurrent)
            {
                if ((AF_INET == lCurrent->ai_family) || (AF_INET6 == lCurrent->ai_family))
                {
                    SetInternal(lAddr->ai_addr, static_cast<unsigned int>(lAddr->ai_addrlen));

                    freeaddrinfo(lAddr);

                    mName = aN;

                    return;
                }

                lCurrent = lCurrent->ai_next;
            }

            freeaddrinfo(lAddr);

            KMS_EXCEPTION_WITH_INFO(NETWORK_ADDRESS, "Cannot resolve the network address", aN);
        }

        void Address::SetPortNumber(unsigned int aP)
        {
            if (0xffff < aP)
            {
                KMS_EXCEPTION_WITH_INFO(NETWORK_PORT, "Invalid port number", aP);
            }

            SetPortNumber(static_cast<uint16_t>(aP));
        }

        void Address::UpdateName()
        {
            const uint8_t* lB;

            char lName[32];

            switch (mType)
            {
            case Type::IPv4:
                lB = &mAddress.mIPv4.sin_addr.S_un.S_un_b.s_b1;
                sprintf_s(lName, "%u.%u.%u.%u", lB[0], lB[1], lB[2], lB[3]);
                break;

            case Type::IPv6:
                lB = reinterpret_cast<uint8_t*>(&mAddress.mIPv6.sin6_addr.u.Byte);
                sprintf_s(lName, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", lB[0], lB[1], lB[2], lB[3], lB[4], lB[5], lB[6], lB[7]);
                break;

            default: assert(false);
            }

            mName = lName;
        }

    }
}
