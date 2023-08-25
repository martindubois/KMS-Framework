
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Graph_Point.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Graph/Point.h>

namespace KMS
{
    namespace Graph
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const Point Point::ORIGIN(0, 0);

        Point::Point(uint16_t aX_px, uint16_t aY_px) : mX_px(aX_px), mY_px(aY_px)
        {}

    }
}
