
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Graph/Color.h

#pragma once

namespace KMS
{
    namespace Graph
    {

        class Color final
        {

        public:

            static const Color BLACK;
            static const Color BLUE;
            static const Color GREEN;
            static const Color RED;
            static const Color WHITE;

            // aBlue  First byte of the color information
            Color(const uint8_t* aBlue);

            Color(uint32_t aColor);

            Color(uint8_t aRed, uint8_t aGreen, uint8_t aBlue);

            operator uint32_t () const;

        private:

            uint32_t mColor;

        };

    }
}

