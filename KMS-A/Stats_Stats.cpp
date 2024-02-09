
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Stats_Stats.cpp

// TEST COVERAGE 2023-09-04 Martin Dubois

#include "Component.h"

// ===== C ==================================================================
#include <math.h>

// ===== Includes ===========================================================
#include <KMS/Stats/Stats.h>

namespace KMS
{
    namespace Stats
    {

        Stats::Stats() { Reset(); }

        void Stats::AddData(double aValue)
        {
            assert(mMax >= mMin);

            mCount++;
            mLast = aValue;

            if (1 == mCount)
            {
                mMax = aValue;
                mMin = aValue;
            }
            else
            {
                if (mMax < aValue)
                {
                    mMax = aValue;
                }
                else if (mMin > aValue)
                {
                    mMin = aValue;
                }
            }

            mSum  += aValue;
            mSum2 += aValue * aValue;
        }

        double Stats::GetDataAverage() const
        {
            double lResult = mSum;

            if (1 < mCount)
            {
                double lDiv = mCount;

                lResult /= lDiv;
            }

            return lResult;
        }

        unsigned int Stats::GetDataCount () const { return mCount; }
        double       Stats::GetDataLast  () const { return mLast; }
        double       Stats::GetDataMax   () const { return mMax; }
        double       Stats::GetDataMin   () const { return mMin; }
        double       Stats::GetDataStdDev() const { return sqrt(GetDataVariance()); }

        double Stats::GetDataVariance() const
        {
            double lResult;

            if (1 < mCount)
            {
                double lDivA = mCount;
                double lDivB = mCount - 1;

                lResult = (mSum2 - ((mSum * mSum) / lDivA)) / lDivB;
            }
            else
            {
                lResult = 0.0;
            }

            return lResult;
        }

        void Stats::Reset()
        {
            mCount = 0;
            mLast  = 0.0;
            mMax   = 0.0;
            mMin   = 0.0;
            mSum   = 0.0;
            mSum2  = 0.0;
        }

    }
}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const Stats::Stats& aIn)
{
    aOut << "Data count      : " << aIn.GetDataCount() << "\n";
    aOut << "Last data       : " << aIn.GetDataLast () << "\n";
    aOut << "Min ; Avg ; Max : " << aIn.GetDataMin() << " ; " << aIn.GetDataAverage() << " ; " << aIn.GetDataMax() << "\n";
    aOut << "Var. ; Std Dev. : " << aIn.GetDataVariance() << " ; " << aIn.GetDataStdDev() << std::endl;

    return aOut;
}
