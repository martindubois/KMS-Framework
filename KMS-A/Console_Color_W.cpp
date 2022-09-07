
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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

// Constants
// //////////////////////////////////////////////////////////////////////////

static const uint8_t CODES[static_cast<unsigned int>(KMS::Console::Color::COLOR_QTY)] = { 0x09, 0x0a, 0x0c, 0x0e, 0x07 };

// Static variable
// //////////////////////////////////////////////////////////////////////////

static HANDLE sConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Public
// //////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& aOut, const KMS::Console::Color& aC)
{
    assert(INVALID_HANDLE_VALUE != sConsole);

    if ((&aOut == &std::cout) || (&aOut == &std::cerr))
    {
        SetConsoleTextAttribute(sConsole, CODES[static_cast<unsigned int>(aC)]);
    }

    return aOut;
}
