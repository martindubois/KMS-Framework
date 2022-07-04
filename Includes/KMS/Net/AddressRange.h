
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/AddressRange.h

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/Net/Address.h>

namespace KMS
{
    namespace Net
    {

        class AddressRange
        {

        public:

            AddressRange();

            // aAR  Format  A.B.C.D
            //              A.B.C.D/M
            //              A.B.C.D.E.F.G.H
            //              A.B.C.D.E.F.G.H/M
            AddressRange(const char* aAR);

            AddressRange(const Address& aAR);

            // aAR  See the constructor
            const AddressRange& operator = (const char* aAR);
            const AddressRange& operator = (const Address& aA);

            bool operator == (const Address& aA) const;
            bool operator == (const AddressRange& aAR) const;

            void Get(char* aOut, unsigned int aOutSize_byte) const;

            Address::Type GetType() const;

            void Set(const Address& aA, unsigned int aMask);

            void SetType(Address::Type aType);

        private:

            void Construct();

            unsigned int GetSize() const;

            void ResetMask();

            void Set(const char* aAR);

            void SetMask(unsigned int aBits);

            uint8_t mData[8];
            uint8_t mMask[8];

            Address::Type mType;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Net::AddressRange& aAR);
