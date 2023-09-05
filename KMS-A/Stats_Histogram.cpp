
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Stats_Stats.cpp

// TEST COVERAGE  2023-09-04  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Stats/Histogram.h>

namespace KMS
{
    namespace Stats
    {

        Histogram::Histogram(double aMin, double aMax, unsigned int aZoneQty)
            : mMax(aMax)
            , mMin(aMin)
            , mZoneQty(aZoneQty)
            , mZoneWidth((aMax - aMin) / static_cast<double>(aZoneQty))
        {
            assert(aMin < aMax);
            assert(1 < mZoneQty);

            mZones = new unsigned int[aZoneQty];

            Reset();
        }

        Histogram::~Histogram()
        {
            assert(nullptr != mZones);

            delete[] mZones;
        }

        unsigned int Histogram::GetAboveMaxCount() const { return mAboveMaxCount; }
        unsigned int Histogram::GetBelowMinCount() const { return mBelowMinCount; }

        unsigned int Histogram::GetZoneCount(unsigned int aIndex) const
        {
            assert(nullptr != mZones);

            return mZones[aIndex];
        }

        double Histogram::GetZoneMax(unsigned int aIndex) const
        {
            assert(0.0 < mZoneWidth);
            assert(aIndex < mZoneQty);

            double lMult = mZoneQty - aIndex - 1;

            return mMax - lMult * mZoneWidth;
        }

        double Histogram::GetZoneMin(unsigned int aIndex) const
        {
            assert(0.0 < mZoneWidth);

            double lMult = aIndex;

            return mMin + lMult * mZoneWidth;
        }

        unsigned int Histogram::GetZoneQty() const { return mZoneQty; }

        double Histogram::GetZoneWidth() const { return mZoneWidth; }

        const unsigned int* Histogram::GetZones() const { return mZones; }

        // ===== Stats ======================================================

        void Histogram::AddData(double aValue)
        {
            assert(1 < mZoneQty);
            assert(0.0 < mZoneWidth);
            assert(nullptr != mZones);

            Stats::AddData(aValue);

            if (mMin > aValue)
            {
                mBelowMinCount++;
            }
            else
            {
                double lValue = (aValue - mMin) / mZoneWidth;

                unsigned int lIndex = static_cast<unsigned int>(lValue);

                if (mZoneQty <= lIndex)
                {
                    mAboveMaxCount++;
                }
                else
                {
                    mZones[lIndex]++;
                }
            }
        }

        void Histogram::Reset()
        {
            assert(1 < mZoneQty);
            assert(nullptr != mZones);

            mAboveMaxCount = 0;
            mBelowMinCount = 0;

            memset(mZones, 0, sizeof(unsigned int) * mZoneQty);

            Stats::Reset();
        }

    }
}
