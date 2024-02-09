
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Stats_Entry.cpp

// TEST COVERAGE 2023-08-06 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dbg/Stats.h>

#include <KMS/Dbg/Stats_Entry.h>

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Stats_Entry::Stats_Entry(const char* aName) : mName(aName)
        {
            assert(nullptr != aName);

            gStats.AddEntry(this);
        }

        Stats_Entry::~Stats_Entry() {}

        const char* Stats_Entry::GetName() const { return mName; }

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Dbg::Stats_Entry& aIn) { aIn.Display(aOut); return aOut; }
