
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Stats/Histrogram.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Stats/Stats.h>

namespace KMS
{
    namespace Stats
    {

        class Histogram : public Stats
        {

        public:

            Histogram(double aMin, double aMax, unsigned int aZoneQty);

            ~Histogram();

            unsigned int GetAboveMaxCount() const;
            unsigned int GetBelowMinCount() const;

            unsigned int GetZoneCount(unsigned int aIndex) const;

            double GetZoneMax(unsigned int aIndex) const;
            double GetZoneMin(unsigned int aIndex) const;

            unsigned int GetZoneQty() const;

            double GetZoneWidth() const;

            const unsigned int* GetZones() const;

            // ===== Stats ==================================================
            void AddData(double aValue);
            void Reset();

        private:

            NO_COPY(Histogram);

            unsigned int mAboveMaxCount;
            unsigned int mBelowMinCount;

            double mMax;
            double mMin;

            unsigned int* mZones;

            unsigned int mZoneQty;

            double mZoneWidth;

        };

    }
}
