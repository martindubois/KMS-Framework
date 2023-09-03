
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Address.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <WS2tcpip.h>

// ===== Includes ===========================================================
#include <KMS/SafeAPI.h>

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
            mSize_byte = 0;
            mType = Type::UNKNOWN;

            memset(&mAddress, 0, sizeof(mAddress));
        }

        uint16_t Address::GetPortNumber() const
        {
            uint16_t lPort = 0;

            switch (mType)
            {
            case Type::IPv4: lPort = ntohs(mAddress.mIPv4.sin_port ); break;
            case Type::IPv6: lPort = ntohs(mAddress.mIPv6.sin6_port); break;

            default: assert(false);
            }

            return lPort;
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

        // Return nullptr it the address type is UNKNOWN.
        const void* Address::GetBinary() const
        {
            const void* lResult = nullptr;

            switch (mType)
            {
            case Type::IPv4: lResult = &mAddress.mIPv4.sin_addr; break;
            case Type::IPv6: lResult = &mAddress.mIPv6.sin6_addr; break;
            }

            return lResult;
        }

        int Address::GetInternalFamily() const { return mAddress.mBase.sa_family; }

        int Address::GetInternalSize() const { return mSize_byte; }

        void Address::SetAddress(const char* aA)
        {
            assert(nullptr != aA);

            unsigned int lB[4];

            if (4 == sscanf_s(aA, "%u.%u.%u.%u", lB + 0, lB + 1, lB + 2, lB + 3)) { SetIPv4(lB); return; }

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
            case sizeof(mAddress.mIPv4) : mType = Type::IPv4; break;
            case sizeof(mAddress.mIPv6) : mType = Type::IPv6; break;

            default: assert(false);
            }

            mSize_byte = aAddrSize_byte;

            UpdateName();
        }

        void Address::SetPortNumber(unsigned int aP)
        {
            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid port number", aP);
            KMS_EXCEPTION_ASSERT(0xffff >= aP, RESULT_INVALID_PORT, lMsg, "");

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
            assert(nullptr != aN);

            addrinfo* lAddr;

            auto lRet = getaddrinfo(aN, NULL, NULL, &lAddr);

            char lMsg[64 + NAME_LENGTH];
            sprintf_s(lMsg, "Cannot resolve the network address \"%s\"", aN);
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_ADDRESS_RESOLUTION_FAILED, lMsg, lRet);

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

                lCurrent = lCurrent->ai_next;
            }

            freeaddrinfo(lAddr);

            KMS_EXCEPTION(RESULT_ADDRESS_RESOLUTION_FAILED, lMsg, "");
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
