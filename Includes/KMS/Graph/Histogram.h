
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Graph/Histogram.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Graph/Bitmap.h>
#include <KMS/Stats/Histogram.h>

namespace KMS
{
    namespace Graph
    {

        class Histogram : public Stats::Histogram
        {

        public:

            Histogram(double aMin, double aMax, unsigned int aZoneQty);

            void Prepare();

            Bitmap mBitmap;

            // ===== Stats::Histogram =======================================
            void AddData(double aValue);
            void Reset();

        private:

            NO_COPY(Histogram);

            void Clear();

            void Draw();

            unsigned int mDivY;

            unsigned int mStepX_px;

            unsigned int mX0_px;
            unsigned int mX1_px;

        };

    }
}
