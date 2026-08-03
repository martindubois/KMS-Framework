
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

            QTY
        };

        const char* Operator_NAMES[];
        const char* Operator_SYMBOLS[];

        int64_t  Operator_Eval( int64_t aCurrent, Operator aOp,  int64_t aValue);
        uint64_t Operator_Eval(uint64_t aCurrent, Operator aOp, uint64_t aValue);
        double   Operator_Eval(double   aCurrent, Operator aOp, double   aValue);

    }
}
