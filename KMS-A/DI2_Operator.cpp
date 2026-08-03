
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

        int64_t Operator_Eval(int64_t aCurrent, Operator aOp, int64_t aValue)
        {
            int64_t lResult;

            switch (aOp)
            {
            case Operator::ASSIGN: lResult = aValue; break;

            case Operator::ADD : lResult = aCurrent + aValue; break;
            case Operator::AND : lResult = aCurrent & aValue; break;
            case Operator::MULT: lResult = aCurrent * aValue; break;
            case Operator::OR  : lResult = aCurrent | aValue; break;
            case Operator::SUB : lResult = aCurrent - aValue; break;
            case Operator::XOR : lResult = aCurrent ^ aValue; break;

            case Operator::DIV:
                KMS_EXCEPTION_ASSERT(0 != aValue, RESULT_INVALID_VALUE, "Cannot divide by 0", "");
                lResult = aCurrent / aValue;
                break;

            default: KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid operator", "");
            }

            return lResult;
        }

        uint64_t Operator_Eval(uint64_t aCurrent, Operator aOp, uint64_t aValue)
        {
            uint64_t lResult;

            switch (aOp)
            {
            case Operator::ASSIGN: lResult = aValue; break;

            case Operator::ADD : lResult = aCurrent + aValue; break;
            case Operator::AND : lResult = aCurrent & aValue; break;
            case Operator::MULT: lResult = aCurrent * aValue; break;
            case Operator::OR  : lResult = aCurrent | aValue; break;
            case Operator::SUB : lResult = aCurrent - aValue; break;
            case Operator::XOR : lResult = aCurrent ^ aValue; break;

            case Operator::DIV:
                KMS_EXCEPTION_ASSERT(0 != aValue, RESULT_INVALID_VALUE, "Cannot divide by 0", "");
                lResult = aCurrent / aValue;
                break;

            default: KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid operator", "");
            }

            return lResult;
        }

        double Operator_Eval(double aCurrent, Operator aOp, double aValue)
        {
            double lResult;

            switch (aOp)
            {
            case Operator::ASSIGN: lResult = aValue; break;

            case Operator::ADD : lResult = aCurrent + aValue; break;
            case Operator::MULT: lResult = aCurrent * aValue; break;
            case Operator::SUB : lResult = aCurrent - aValue; break;

            case Operator::DIV:
                KMS_EXCEPTION_ASSERT(0.0 != aValue, RESULT_INVALID_VALUE, "Cannot divide by 0", "");
                lResult = aCurrent / aValue;
                break;

            default: KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid operator", "");
            }

            return lResult;
        }

    }
}
