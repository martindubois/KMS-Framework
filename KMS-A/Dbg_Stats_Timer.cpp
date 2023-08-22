
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Stats_Timer.cpp

// TEST COVERAGE  2023-08-06  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== C ==================================================================
#include <float.h>

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

        Stats_Timer::Stats_Timer(const char* aName)
            : Stats_Value<double>(aName, "s"), mStart_s(DBL_MAX)
        {}

        void Stats_Timer::Start() { mStart_s = GetNow_s(); }

        void Stats_Timer::Stop()
        {
            auto lNow_s = GetNow_s();
            if (mStart_s <= lNow_s)
            {
                Set(lNow_s - mStart_s);
            }

            mStart_s = DBL_MAX;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Stats_Entry ================================================

        void Stats_Timer::Display(std::ostream& aOut) const
        {
            auto lCount = GetCount();
            if (0 < lCount)
            {
                aOut << GetName() << " : ";

                auto   lAvg = GetAverage();
                double lDiv = 1.0;

                auto lUnit = "s";

                if (60.0 < lAvg)
                {
                    // NOT TESTED
                    lAvg /= 60.0;
                    lDiv *= 60.0;
                    lUnit = "m";

                    if (60.0 < lAvg)
                    {
                        lAvg /= 60.0;
                        lDiv *= 60.0;
                        lUnit = "h";
                    }
                }

                aOut << (GetLast() / lDiv) << " " << lUnit;

                if (1 < lCount)
                {
                    aOut << " ( " << (GetMin() / lDiv) << " ; " << lAvg << " ; " << (GetMax() / lDiv);
                    aOut << " ; n = " << lCount << " ; Std.Dev. = " << ( GetStdDev() / lDiv ) << " )";
                }

                aOut << std::endl;
            }
        }
    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

double GetNow_s()
{
    auto lNow_100ns = OS::GetSystemTime();

    auto lResult_s = static_cast<double>(lNow_100ns);

    lResult_s /= 10000000;

    return lResult_s;
}
