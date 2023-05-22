
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_AddressRangeList.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/AddressRangeList.h>

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void AddressRangeList::Add(const AddressRange& aAR) { mInternal.push_back(aAR); }

        void AddressRangeList::Clear() { mInternal.clear(); }

        bool AddressRangeList::IsInRanges(const Address& aA) const
        {
            for (auto& lAR : mInternal)
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
