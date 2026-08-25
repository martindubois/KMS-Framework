
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/String.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Input.h>
#include <KMS/DI2/IType.h>
#include <KMS/DI2/Output.h>
#include <KMS/Enum.h>
#include <KMS/Result.h>

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

        extern const std::regex String_REGEX_OP_VALUE_0_C2;
        extern const std::regex String_REGEX_OP_VALUE_1_C2;
        extern const std::regex String_REGEX_VALUE_0_C1;
        extern const std::regex String_REGEX_VALUE_1_C1;

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
            assert(false);
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

            auto lData = reinterpret_cast<char*>(aData);

            std::smatch lMatch;

            if (   aInput->Next_Try(String_REGEX_OP_VALUE_0_C2, lMatch)
                || aInput->Next_Try(String_REGEX_OP_VALUE_1_C2, lMatch))
            {
                KMS::Enum<Operator, Operator_SYMBOLS> lOp(lMatch[1].str().c_str());

                auto lCurrentLen = strlen(lData);
                auto lValueLen   = lMatch[2].length();

                switch (lOp)
                {
                case Operator::ASSIGN:
                    KMS_EXCEPTION_ASSERT(N > lValueLen, RESULT_INVALID_FORMAT, "String too long", lMatch[2].str().c_str());

                    memset(aData, 0, N);

                    strcpy_s(lData SizeInfoV(N), lMatch[2].str().c_str());
                    break;

                case Operator::ADD:
                    KMS_EXCEPTION_ASSERT(N > lCurrentLen + lValueLen, RESULT_INVALID_FORMAT, "String too long", lMatch[2].str().c_str());

                    strcpy_s(lData + lCurrentLen SizeInfoV(N - lCurrentLen), lMatch[2].str().c_str());
                    break;

                default: KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid operator for string", "");
                }
            }
            else
            {
                auto lRet = aInput->Next_Try(String_REGEX_VALUE_0_C1, lMatch)
                    ||      aInput->Next_Try(String_REGEX_VALUE_1_C1, lMatch);
                KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_FORMAT, "Invalid string format", "");

                KMS_EXCEPTION_ASSERT(N > lMatch[1].length(), RESULT_INVALID_FORMAT, "String too long", lMatch[1].str().c_str());

                memset(aData, 0, N);

                strcpy_s(lData SizeInfoV(N), lMatch[1].str().c_str());
            }
        }

        // "Value"
        template <unsigned int N>
        void String<N>::Decode_JSON(void* aData, Input* aInput) const
        {
            assert(false);
        }

    }
}
