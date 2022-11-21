
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Console_Color_L.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

static const char* CODES[static_cast<unsigned int>(Console::Color::COLOR_QTY)] = { "\x1B[34m", "\x1B[32m", "\x1B[31m", "\x1B[33m", "\x1B[37m" };

// Public
// //////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& aOut, const Console::Color& aC)
{
    if ((&aOut == &std::cout) || (&aOut == &std::cerr))
    {
        aOut << CODES[static_cast<unsigned int>(aC)];
    }

    return aOut;
}
