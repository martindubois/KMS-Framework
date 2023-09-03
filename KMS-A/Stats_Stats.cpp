
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Stats_Stats.cpp

#include "Component.h"

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

    }
}
