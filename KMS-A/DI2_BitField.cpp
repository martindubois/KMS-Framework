
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_BitField.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <regex>

// ===== Includes ===========================================================
#include <KMS/DI2/BitField.h>

// Macros
// //////////////////////////////////////////////////////////////////////////

#define REGEX_BEGIN "^"
#define REGEX_DEC   "(\\d+)"
#define REGEX_DOT   "\\."
#define REGEX_HEX   "(0x[0-9A-Fa-f]+)"
#define REGEX_NAME  "(\\w+)"
#define REGEX_OP    "([\\+\\-\\*/\\|&\\^]{0,1}=)"
#define REGEX_SPACE "\\s*"

namespace KMS
{
    namespace DI2
    {

        const std::regex BitField_REGEX_BEGIN(REGEX_BEGIN REGEX_SPACE "={0,1}" REGEX_SPACE "\\{" REGEX_SPACE);

        const std::regex BitField_REGEX_END(REGEX_BEGIN REGEX_SPACE "\\}" REGEX_SPACE);

        const std::regex BitField_REGEX_FIELD_END(REGEX_BEGIN REGEX_SPACE ";" REGEX_SPACE);

        const std::regex BitField_REGEX_DEC(REGEX_BEGIN REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_OP REGEX_SPACE REGEX_DEC REGEX_SPACE);
        const std::regex BitField_REGEX_HEX(REGEX_BEGIN REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_OP REGEX_SPACE REGEX_HEX REGEX_SPACE);

        const std::regex BitField_REGEX_FIELD_OP_VALUE_DEC_0(REGEX_BEGIN REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_OP REGEX_SPACE REGEX_DEC REGEX_SPACE);
        const std::regex BitField_REGEX_FIELD_OP_VALUE_DEC_1(REGEX_BEGIN REGEX_DOT   REGEX_NAME REGEX_SPACE REGEX_OP REGEX_SPACE REGEX_DEC REGEX_SPACE);
        const std::regex BitField_REGEX_FIELD_OP_VALUE_HEX_0(REGEX_BEGIN REGEX_SPACE REGEX_NAME REGEX_SPACE REGEX_OP REGEX_SPACE REGEX_HEX REGEX_SPACE);
        const std::regex BitField_REGEX_FIELD_OP_VALUE_HEX_1(REGEX_BEGIN REGEX_DOT   REGEX_NAME REGEX_SPACE REGEX_OP REGEX_SPACE REGEX_HEX REGEX_SPACE);

        const std::regex BitField_REGEX_OP_VALUE_DEC(REGEX_BEGIN REGEX_SPACE REGEX_OP REGEX_SPACE REGEX_DEC REGEX_SPACE);
        const std::regex BitField_REGEX_OP_VALUE_HEX(REGEX_BEGIN REGEX_SPACE REGEX_OP REGEX_SPACE REGEX_HEX REGEX_SPACE);

        const std::regex BitField_REGEX_VALUE_DEC(REGEX_BEGIN REGEX_SPACE REGEX_DEC REGEX_SPACE);
        const std::regex BitField_REGEX_VALUE_HEX(REGEX_BEGIN REGEX_SPACE REGEX_HEX REGEX_SPACE);

    }
}
