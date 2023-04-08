
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Stats_Timer.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/OS.h>

#include <KMS/Dbg/Stats_Timer.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static double GetNow_s();

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Stats_Timer::Stats_Timer(const char* aName) : Stats_Value<double>(aName, "s"), mStart_s(0.0) {}

        void Stats_Timer::Start()
        {
            assert(0.0 == mStart_s);

            mStart_s = GetNow_s();
        }

        void Stats_Timer::Stop()
        {
            double lNow_s = GetNow_s();
            if (mStart_s <= lNow_s)
            {
                Set(lNow_s - mStart_s);
            }

            mStart_s = 0.0;
        }

        // ===== Stats_Entry ================================================

        const char* Stats_Timer::GetType() const { return "Stats_Timer"; }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

double GetNow_s()
{
    uint64_t lNow_100ns = OS::GetSystemTime();

    double lResult_s = static_cast<double>(lNow_100ns);

    lResult_s /= 10000000;

    return lResult_s;
}
