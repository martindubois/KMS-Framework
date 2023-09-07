
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Stats/Stats.h

#pragma once

namespace KMS
{
    namespace Stats
    {

        class Stats
        {

        public:

            Stats();

            void AddData(double aValue);

            double GetDataAverage() const;

            unsigned int GetDataCount() const;

            double GetDataLast() const;

            double GetDataMax() const;
            double GetDataMin() const;

            double GetDataStdDev() const;

            double GetDataVariance() const;

            void Reset();

        private:

            unsigned int mCount;

            double mLast;
            double mMax;
            double mMin;
            double mSum;
            double mSum2;

        };

    }
}

std::ostream& operator << (std::ostream&, const KMS::Stats::Stats& aIn);
