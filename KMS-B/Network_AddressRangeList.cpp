
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Network_AddressRangeList.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Network/AddressRangeList.h>

namespace KMS
{
    namespace Network
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void AddressRangeList::Add(const AddressRange& aAR) { mRanges.push_back(aAR); }

        void AddressRangeList::Clear() { mRanges.clear(); }

        bool AddressRangeList::IsInRanges(const Address& aA) const
        {
            for (const AddressRange& lAR : mRanges)
            {
                if (lAR == aA)
                {
                    return true;
                }
            }

            return false;
        }

    }
}
