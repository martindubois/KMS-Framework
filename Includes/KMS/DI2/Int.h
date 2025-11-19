
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Int.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Output.h>
#include <KMS/DI2/IType.h>

namespace KMS
{
    namespace DI2
    {

        template <typename T>
        class Int final : public IType
        {

        public:

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        };

        extern const Int<int64_t> TYPE_INT64;
        extern const Int<int32_t> TYPE_INT32;
        extern const Int<int16_t> TYPE_INT16;
        extern const Int<int8_t > TYPE_INT8;

        // Value
        template <typename T>
        void Int<T>::Code_ASCII(const void* aData, Output* aOutput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aOutput);

            int64_t lValue = *reinterpret_cast<const T*>(aData);

            aOutput->Token_Append(lValue);
        }

        // Value
        template <typename T>
        void Int<T>::Code_JSON(const void* aData, Output* aOutput) const
        {
            // TODO
        }

        // Operators += -= *= /= |= &= ^=
        //
        // Value
        // op Value
        template <typename T>
        void Int<T>::Decode_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            auto lOp = Operator::ASSIGN;

            auto lTT = aInput->Token_Next(TokenType::INT | TokenType::OPERATOR);
            if (TokenType::OPERATOR == lTT)
            {
                lOp = aInput->Token_GetOperator();

                aInput->Token_Next(TokenType::INT);
            }

            auto lValue = aInput->Token_GetInt();

            int64_t lCurrent = *reinterpret_cast<T*>(aData);

            switch (lOp)
            {
            case Operator::ASSIGN: break;

            case Operator::ADD : lValue = lCurrent + lValue; break;
            case Operator::MULT: lValue = lCurrent * lValue; break;
            case Operator::SUB : lValue = lCurrent - lValue; break;

            case Operator::DIV:
                KMS_EXCEPTION_ASSERT(0 != lValue, RESULT_INVALID_VALUE, "Cannot divide by 0", "");
                lValue = lCurrent / lValue;
                break;

            default: KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid operator", "");
            }

            *reinterpret_cast<T*>(aData) = static_cast<T>(lValue);

            KMS_EXCEPTION_ASSERT(*reinterpret_cast<T*>(aData) == lValue, RESULT_INVALID_VALUE, "Integer value too large", "");
        }

        // Value
        template <typename T>
        void Int<T>::Decode_JSON(void* aData, Input* aInput) const
        {
            // TODO
        }

    }
}
