
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/AddressRangeList.h
// Library   KMS-B

#pragma once

// ===== C++ ================================================================
#include <ostream>
#include <vector>

// ===== Includes ===========================================================
#include <KMS/Net/AddressRange.h>

namespace KMS
{
    namespace Net
    {

        class AddressRangeList final
        {

        public:

            void Add(const AddressRange& aAR);

            void Clear();

            const AddressRange& Get(unsigned int aIndex);

            unsigned int GetCount();

            bool IsInRanges(const Address& aA) const;

            void Remove(const AddressRange* aAR);

        // Internal

            typedef std::vector<AddressRange> Internal;

            std::ostream& Display(std::ostream& aOut) const;

            Internal mInternal;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Net::AddressRangeList& aARL);
