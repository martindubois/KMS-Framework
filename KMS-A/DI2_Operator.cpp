
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Int.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Operator.h>

namespace KMS
{
    namespace DI2
    {

        const char* Operator_NAMES[] =
        {
            "ADD",
            "AND",
            "ASSIGN",
            "DIV",
            "MULT",
            "OR",
            "SUB",
            "XOR",
        };

        const char* Operator_SYMBOLS[] =
        {
            "+=",
            "&=",
            "=",
            "/=",
            "*=",
            "|=",
            "-=",
            "^=",
        };

    }
}
