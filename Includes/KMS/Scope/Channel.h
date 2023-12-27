
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Scope/Channel.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Graph/Color.h>
#include <KMS/Stats/Stats.h>

namespace KMS
{
    namespace Scope
    {

        class Channel
        {

        public:

            static const char* EDGE_TYPE_NAMES[];

            enum class EdgeType
            {
                BOTH = 0,
                FALLING,
                NONE,
                RAISING,

                QTY
            };

            static const Graph::Color COLOR_DEFAULT;
            static const EdgeType     EDGE_TYPE_DEFAULT;
            static const double       SCALE_Y_DEFAULT;
            static const double       TRIG_LEVEL_DEFAULT;
            static const uint16_t     Y_DEFAULT_px;
            static const uint16_t     Y_MAX_px;

            Graph::Color mColor;
            EdgeType     mEdgeType;
            double       mScaleY;
            double       mTrigLevel;
            uint16_t     mY_px;

            Stats::Stats mStats;

        // Internal:

            virtual void Sample(unsigned int* aY_px, bool* aTrig);

        protected:

            Channel();

            void SetSample(double aValue);

        private:

            double mPrevious;

        };

    }
}
