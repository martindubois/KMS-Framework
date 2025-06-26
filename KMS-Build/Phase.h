
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Phase.h

#pragma once

enum class Phase
{
    VERIFY      , // Verify the tools are installed
    EDIT        , // Edit text files to change version
    PRE_BUILD   , // Execute pre-build commands
    COMPILE     , // Compile
    TEST        , // Execute tests
    GENERATE_DOC, // Generate documentation
    PACKAGE     , // Package files
    SIGN        , // Sign driver packages
    EXPORT      , // Export packages

    NONE
};

inline Phase& operator ++ (Phase& aPhase) { return aPhase = static_cast<Phase>(static_cast<int>(aPhase) + 1); }
