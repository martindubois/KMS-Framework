
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Graph_Color.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Graph/Color.h>

namespace KMS
{
    namespace Graph
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const Color Color::BLACK(static_cast<uint32_t>(0x00000000));
        const Color Color::BLUE (0x000000ff);
        const Color Color::GREEN(0x0000ff00);
        const Color Color::RED  (0x00ff0000);
        const Color Color::WHITE(0x00ffffff);

        Color::Color(const uint8_t* aBlue)
        {
            assert(nullptr != aBlue);

            mColor = *reinterpret_cast<const uint32_t*>(aBlue) & WHITE.mColor;
        }

        Color::Color(uint32_t aColor) : mColor(aColor)
        {}

        Color::Color(uint8_t aRed, uint8_t aGreen, uint8_t aBlue)
        {
            mColor = aRed;

            mColor <<= 8;
            mColor |= aGreen;
            mColor <<= 8;
            mColor |= aBlue;
        }

        Color::operator uint32_t () const { return mColor; }

    }
}
