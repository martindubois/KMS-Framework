
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Address.h

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== C++ ================================================================
#include <ostream>
#include <string>

// ===== Windows ============================================================
#include <winsock2.h>
#include <ws2ipdef.h>

namespace KMS
{
    namespace Net
    {

        class Address
        {

        public:

            enum class Type
            {
                IPv4,
                IPv6,

                UNKNOWN,
            };

            static const uint16_t PORT_ANY;

            Address();

            // aA  Format  A.B.C.D
            //             A.B.C.D:Port
            //             IPv4
            //             IPv4:Name
            //             IPv4:Name:Port
            //             IPv6:Name
            //             IPv6:Name:Port
            //             Name
            //             Name:Port
            Address(const char* aA);

            // aA  See the constructor
            const Address& operator = (const char* aA);

            bool operator == (const Address& aA);

            void Clear();

            // Format Name:Port
            //        A.B.C.D:Port
            //        A:B:C:D:E:F:G:H:Port
            void Get(char* aOut, unsigned int aOutSize_byte) const;

            const char* GetName      () const;
            uint16_t    GetPortNumber() const;
            Type        GetType      () const;

            bool IsBroadcast() const;
            bool IsMulticast() const;

            void SetBroadcast_IPv4();
            void SetBroadcast_IPv6();

            void SetLocal_IPv4();
            void SetLocal_IPv6();

            void SetMulticast_IPv4();
            void SetMulticast_IPv6();

            // The address type must be set before calling this method.
            void SetPortNumber(uint16_t aPort);

            void SetType(Type aType);

        // Internal

            static const unsigned int INTERNAL_SIZE_byte;

            operator sockaddr* ();

            const void* GetBinary() const;

            int GetInternalFamily() const;

            int GetInternalSize() const;

            void SetBinary(const void* aIn, unsigned int aInSize_byte);

            void SetInternal(const struct sockaddr* aAddr, unsigned int aAddrSize_byte);

            void SetInternalSize(unsigned int aAddrSize_byte);

        private:

            void Set(const char* aA);
            void SetAddress(const char* aA);
            void SetIPv4(const unsigned int* aA);
            void SetName(const char* aN);
            void SetPortNumber(unsigned int aP);

            void UpdateName();

            union
            {
                struct sockaddr     mBase;
                struct sockaddr_in  mIPv4;
                struct sockaddr_in6 mIPv6;
            }
            mAddress;

            std::string mName;

            unsigned int mSize_byte;

            Type mType;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Net::Address& aA);

std::ostream& operator << (std::ostream& aOut, const KMS::Net::Address::Type& aT);
