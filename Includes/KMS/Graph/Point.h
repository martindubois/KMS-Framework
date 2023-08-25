
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Graph/Point.h

#pragma once

namespace KMS
{
    namespace Graph
    {

        class Point
        {

        public:

            static const Point ORIGIN;

            Point(uint16_t aX_px, uint16_t aT_px);

            uint16_t mX_px;
            uint16_t mY_px;

        };

    }
}

