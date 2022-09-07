
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Console/Color.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

namespace KMS
{
    namespace Console
    {

        enum class Color
        {
            BLUE,
            GREEN,
            RED,
            YELLOW,
            WHITE,

            COLOR_QTY,
        };

    }
}

std::ostream & operator << (std::ostream & aOut, const KMS::Console::Color & aC);
