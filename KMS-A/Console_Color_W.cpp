
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Console_Color_W.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

static const uint8_t CODES[static_cast<unsigned int>(Console::Color::COLOR_QTY)] = { 0x09, 0x0a, 0x0c, 0x0e, 0x07 };

// Public
// //////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& aOut, const Console::Color& aC)
{
    if ((&aOut == &std::cout) || (&aOut == &std::cerr))
    {
        auto lConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        assert(INVALID_HANDLE_VALUE != lConsole);

        SetConsoleTextAttribute(lConsole, CODES[static_cast<unsigned int>(aC)]);
    }

    return aOut;
}
