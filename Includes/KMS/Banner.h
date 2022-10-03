
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Banner.h

#pragma once

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>

// Macros
// //////////////////////////////////////////////////////////////////////////

#define KMS_BANNER(P,C)                                                     \
    std::cerr << KMS::Console::Color::BLUE << P << " - " << C << std::endl; \
    std::cerr << "Version " << VERSION << std::endl;                        \
    std::cerr << KMS::Console::Color::WHITE << std::endl;
