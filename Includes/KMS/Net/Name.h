
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Name.h
// Library   KMS-B

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/Configurable.h>
#include <KMS/Net/Protocol.h>

namespace KMS
{
    namespace Net
    {

        class Name
        {

        public:

            // Return  nullptr  No address to return
            const sockaddr* GetAddress() const;

            // Return  0  No address
            unsigned int GetAddressLength() const;

            // Return  nullptr  No address to return
            const sockaddr* GetIPv4() const;

            // Return  nullptr  No address to return
            const sockaddr* GetIPv6() const;

            // Return  0  No port
            uint16_t GetPortNumber() const;

            // Return  UNKNOWN  No protocol
            Protocol GetProtocol() const;

            void GetURL(char* aOut, unsigned int aOutSize_byte) const;

            bool IsAddressValid() const;

            bool IsIPv4Valid() const;

            bool IsIPv6Valid() const;

            bool IsPortNumberValid() const;

            bool IsProtocolValid() const;

            void SetIPv4(const uint8_t* aIn);

            void SetIPv4(const char* aIn);

            void SetIPv6(const uint8_t* aIn);

            void SetName(const char* aIn);

            void SetPortNumber(uint16_t aIn);

            void SetProtocol(Protocol aIn);

            void SetURL(const char* aIn, char* aOut = nullptr, unsigned int aOutSize_byte = 0);

            void Resolve();

            KMS_Config_METHODS(Name);

        private:

            struct
            {
                uint32_t mIPv4       : 1;
                uint32_t mIPv6       : 1;
                uint32_t mName       : 1;
                uint32_t mPortNumber : 1;

                uint32_t mReserved0 : 28;
            }
            mFlags;

            sockaddr_in  mIPv4;
            sockaddr_in6 mIPv6;
            char         mName[NAME_LENGTH];
            uint16_t     mPortNumber;
            Protocol     mProtocol;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        inline uint16_t Name::GetPortNumber() const { return mPortNumber; }

        inline Protocol Name::GetProtocol() const { return mProtocol; }

        inline bool Name::IsAddressValid() const { return mFlags.mIPv4 || mFlags.mIPv6; }

        inline bool Name::IsIPv4Valid() const { return mFlags.mIPv4; }

        inline bool Name::IsIPv6Valid() const { return mFlags.mIPv6; }

        inline bool Name::IsPortNumberValid() const { return mFlags.mPortNumber; }

        inline bool Name::IsProtocolValid() const { return Protocol::UNKNOWN != mProtocol; }

        inline void Name::SetProtocol(Protocol aIn) { mProtocol = aIn; }

    }
}
