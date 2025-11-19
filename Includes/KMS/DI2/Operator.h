
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Operator.h
// Library   KMS-A

#pragma once

namespace KMS
{
    namespace DI2
    {

        enum class Operator
        {
            ADD = 0,
            AND,
            ASSIGN,
            DIV,
            MULT,
            OR,
            SUB,
            XOR,
        };

        const char* ToString(Operator aOpertor);

    }
}
