
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/String.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Input.h>
#include <KMS/DI2/IType.h>
#include <KMS/DI2/Output.h>

namespace KMS
{
    namespace DI2
    {

        template <unsigned int N>
        class String : public IType
        {

        public:

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        };

        // "Value"
        template <unsigned int N>
        void String<N>::Code_ASCII(const void* aData, Output* aOutput) const
        {
            aOutput->Token_Append(TokenType::QUOTED, reinterpret_cast<const char*>(aData));
        }

        // "Value"
        template <unsigned int N>
        void String<N>::Code_JSON(const void* aData, Output* aOutput) const
        {
            // TODO
        }

        // Operators = +=
        //
        // Value
        // "Value"
        // op Value
        // op "Value"
        template <unsigned int N>
        void String<N>::Decode_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            auto lOp = Operator::ASSIGN;

            auto lTT = aInput->Token_Next(TokenType::OPERATOR | TokenType::QUOTED | TokenType::STRING);
            if (TokenType::OPERATOR == lTT)
            {
                lOp = aInput->Token_GetOperator();

                aInput->Token_Next(TokenType::QUOTED | TokenType::STRING);
            }

            auto lData = reinterpret_cast<char*>(aData);
            auto lLen  = static_cast<unsigned int>(strlen(lData));

            switch (lOp)
            {
            case Operator::ASSIGN: aInput->Token_GetText(lData       , N       ); break;
            case Operator::ADD   : aInput->Token_GetText(lData + lLen, N - lLen); break;

            default: KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid operator for string", "");
            }
        }

        // "Value"
        template <unsigned int N>
        void String<N>::Decode_JSON(void* aData, Input* aInput) const
        {
            // TODO
        }

    }
}
