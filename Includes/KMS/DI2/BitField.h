
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/BitField.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

namespace KMS
{
    namespace DI2
    {

        typedef struct
        {
            const char* mName;
            uint64_t    mMask;
            uint8_t     mShift;
        }
        BitField_Field;

        template <typename T, const BitField_Field* F>
        class BitField : public IType
        {

        public:

            BitField(const IType* aType);

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        private:

            void DecodeField_ASCII(void* aData, Input* aInput) const;

            const BitField_Field* FindField(const char* aName) const;

            const IType* mType;

        };

        template <typename T, const BitField_Field* F>
        BitField<T, F>::BitField(const IType* aType) : mType(aType)
        {
            assert(nullptr != aType);
        }

        // Value
        template <typename T, const BitField_Field* F>
        void BitField<T, F>::Code_ASCII(const void* aData, Output* aOutput) const
        {
            assert(nullptr != mType);

            mType->Code_ASCII(aData, aOutput);
        }

        // Value
        template <typename T, const BitField_Field* F>
        void BitField<T, F>::Code_JSON(const void* aData, Output* aOutput) const
        {
            assert(nullptr != mType);

            mType->Code_JSON(aData, aOutput);
        }

        // Operators = += -= *= /= |= &= ^=
        //
        // Value
        // op Value
        // Field0 op Value0
        // .Field0 op Value0
        // { Field0 op Value0; Field1 op Value1; }
        // = { Field0 op Value0; Field1 op Value1; }
        template <typename T, const BitField_Field* F>
        void BitField<T, F>::Decode_ASCII(void* aData, Input* aInput) const
        {
            if (aInput->Char_Next_Try('='))
            {
                if (aInput->Char_Next_Try('{'))
                {
                    while (!aInput->Char_Next_Try('}'))
                    {
                        DecodeField_ASCII(aData, aInput);

                        aInput->Char_Next(';');
                    }
                }
                else
                {
                    aInput->Previous();
                    mType->Decode_ASCII(aData, aInput);
                }
            }
            else if (aInput->Char_Next_Try('{'))
            {
                while (!aInput->Char_Next_Try('}'))
                {
                    DecodeField_ASCII(aData, aInput);

                    aInput->Char_Next(';');
                }
            }
            else if (aInput->Char_Next_Try('.'))
            {
                DecodeField_ASCII(aData, aInput);
            }
            else
            {
                auto lTT = aInput->Token_Next(TokenType::NAME | TokenType::OPERATOR | TokenType::UINT);
                switch (lTT)
                {
                case TokenType::NAME:
                    aInput->Previous();

                    DecodeField_ASCII(aData, aInput);
                    break;

                case TokenType::OPERATOR:
                case TokenType::UINT:
                    aInput->Previous();
                    mType->Decode_ASCII(aData, aInput);
                    break;

                default: assert(false);
                }
            }
        }

        // Value
        template <typename T, const BitField_Field* F>
        void BitField<T, F>::Decode_JSON(void* aData, Input* aInput) const
        {
            assert(nullptr != mType);

            mType->Decode_JSON(aData, aInput);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        template <typename T, const BitField_Field* F>
        void BitField<T, F>::DecodeField_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            char lName[NAME_LENGTH];

            aInput->Token_Next(TokenType::NAME);
            aInput->Token_GetText(lName, sizeof(lName));
            auto lField = FindField(lName);
            aInput->Token_Next(TokenType::OPERATOR);
            auto lOp = aInput->Token_GetOperator();
            aInput->Token_Next(TokenType::UINT);
            auto lFieldValue = aInput->Token_GetUInt();

            uint64_t lCurrent = *reinterpret_cast<T*>(aData);
            uint64_t lCurrentField = lCurrent & lField->mMask;

            lCurrentField >>= lField->mShift;

            switch (lOp)
            {
            case Operator::ASSIGN: break;

            case Operator::ADD : lFieldValue = lCurrentField + lFieldValue; break;
            case Operator::AND : lFieldValue = lCurrentField & lFieldValue; break;
            case Operator::MULT: lFieldValue = lCurrentField * lFieldValue; break;
            case Operator::OR  : lFieldValue = lCurrentField | lFieldValue; break;
            case Operator::SUB : lFieldValue = lCurrentField - lFieldValue; break;
            case Operator::XOR : lFieldValue = lCurrentField ^ lFieldValue; break;

            case Operator::DIV:
                KMS_EXCEPTION_ASSERT(0 != lFieldValue, RESULT_INVALID_VALUE, "Cannot divide by 0", "");
                lFieldValue = lCurrentField / lFieldValue;
                break;

            default: KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid operator", "");
            }

            lFieldValue <<= lField->mShift;
            lFieldValue &= lField->mMask;

            lCurrent &= ~lField->mMask;
            lCurrent |= lFieldValue;

            *reinterpret_cast<T*>(aData) = static_cast<T>(lCurrent);

        }

        template <typename T, const BitField_Field* F>
        const BitField_Field* BitField<T, F>::FindField(const char* aName) const
        {
            assert(nullptr != aName);

            unsigned int lIndex = 0;

            const BitField_Field* lResult = nullptr;

            for (;;)
            {
                lResult = F + lIndex;

                KMS_EXCEPTION_ASSERT(nullptr != lResult->mName, RESULT_INVALID_NAME, "Invalid field name", aName);

                if (0 == strcmp(lResult->mName, aName))
                {
                    break;
                }

                lIndex++;
            }

            return lResult;
        }

    }
}
