
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/AddressRangeList.h

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

        class AddressRangeList
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

            const Internal& GetInternal();

        private:

            typedef std::vector<AddressRange> Internal;

            Internal mRanges;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Net::AddressRangeList& aARL);
