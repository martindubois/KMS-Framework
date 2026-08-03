
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/BitField.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI2/Input.h>
#include <KMS/DI2/IType.h>
#include <KMS/DI2/Operator.h>

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

            void Decode_ASCII(void* aData, const char* aOp, const char* aValue) const;
            void Decode_ASCII(void* aData, const char* aValue) const;

            void DecodeField_ASCII(void* aData, Input* aInput) const;
            void DecodeField_ASCII(void* aData, const char* aField, const char* aOp, const char* aFieldValue) const;

            const BitField_Field* FindField(const char* aName) const;

            const IType* mType;

        };

        extern const std::regex BitField_REGEX_DEC_C3;
        extern const std::regex BitField_REGEX_HEX_C3;
        extern const std::regex BitField_REGEX_FIELD_END;
        extern const std::regex BitField_REGEX_FIELD_OP_VALUE_DEC_0_C3;
        extern const std::regex BitField_REGEX_FIELD_OP_VALUE_DEC_1_C3;
        extern const std::regex BitField_REGEX_FIELD_OP_VALUE_HEX_0_C3;
        extern const std::regex BitField_REGEX_FIELD_OP_VALUE_HEX_1_C3;
        extern const std::regex BitField_REGEX_GROUP_BEGIN;
        extern const std::regex BitField_REGEX_GROUP_END;
        extern const std::regex BitField_REGEX_OP_VALUE_DEC_C2;
        extern const std::regex BitField_REGEX_OP_VALUE_HEX_C2;
        extern const std::regex BitField_REGEX_VALUE_DEC_C1;
        extern const std::regex BitField_REGEX_VALUE_HEX_C1;

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
            std::smatch lMatch;

            if (   (aInput->Next_Try(BitField_REGEX_FIELD_OP_VALUE_DEC_0_C3, lMatch))
                || (aInput->Next_Try(BitField_REGEX_FIELD_OP_VALUE_DEC_1_C3, lMatch))
                || (aInput->Next_Try(BitField_REGEX_FIELD_OP_VALUE_HEX_0_C3, lMatch))
                || (aInput->Next_Try(BitField_REGEX_FIELD_OP_VALUE_HEX_1_C3, lMatch)))
            {
                DecodeField_ASCII(aData, lMatch[1].str().c_str(), lMatch[2].str().c_str(), lMatch[3].str().c_str());
            }
            else if ((aInput->Next_Try(BitField_REGEX_OP_VALUE_DEC_C2, lMatch))
                ||   (aInput->Next_Try(BitField_REGEX_OP_VALUE_HEX_C2, lMatch)))
            {
                Decode_ASCII(aData, lMatch[1].str().c_str(), lMatch[2].str().c_str());
            }
            else if ((aInput->Next_Try(BitField_REGEX_VALUE_DEC_C1, lMatch))
                ||   (aInput->Next_Try(BitField_REGEX_VALUE_HEX_C1, lMatch)))
            {
                Decode_ASCII(aData, lMatch[1].str().c_str());
            }
            else
            {
                aInput->Next(BitField_REGEX_GROUP_BEGIN);

                while (!aInput->Next_Try(BitField_REGEX_GROUP_END))
                {
                    DecodeField_ASCII(aData, aInput);

                    aInput->Next(BitField_REGEX_FIELD_END);
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
        void BitField<T, F>::Decode_ASCII(void* aData, const char* aOp, const char* aValue) const
        {
            assert(nullptr != aData);

            Enum<Operator, Operator_SYMBOLS> lOp(aOp);

            auto lValue = Operator_Eval(static_cast<uint64_t>(*reinterpret_cast<T*>(aData)), lOp, KMS::Convert::ToUInt64(aValue));

            *reinterpret_cast<T*>(aData) = static_cast<T>(lValue);
        }

        template <typename T, const BitField_Field* F>
        void BitField<T, F>::Decode_ASCII(void* aData, const char* aValue) const
        {
            assert(nullptr != aData);

            auto lValue = KMS::Convert::ToUInt64(aValue);

            *reinterpret_cast<T*>(aData) = static_cast<T>(lValue);
        }

        // Field op Value
        template <typename T, const BitField_Field* F>
        void BitField<T, F>::DecodeField_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aInput);

            std::smatch lMatch;

            auto lRet = aInput->Next_Try(BitField_REGEX_DEC_C3, lMatch) || aInput->Next_Try(BitField_REGEX_HEX_C3, lMatch);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_FORMAT, "Invalide bitfield filed format", "");

            DecodeField_ASCII(aData, lMatch[1].str().c_str(), lMatch[2].str().c_str(), lMatch[3].str().c_str());
        }

        template <typename T, const BitField_Field* F>
        void BitField<T, F>::DecodeField_ASCII(void* aData, const char* aField, const char* aOp, const char* aFieldValue) const
        {
            assert(nullptr != aData);

            Enum<Operator, Operator_SYMBOLS> lOp(aOp);

            auto lField = FindField(aField);
            assert(nullptr != lField);

            uint64_t lCurrent      = *reinterpret_cast<T*>(aData);
            uint64_t lCurrentField = lCurrent & lField->mMask;

            lCurrentField >>= lField->mShift;

            auto lFieldValue = Operator_Eval(lCurrentField, lOp, KMS::Convert::ToUInt64(aFieldValue));

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

            const BitField_Field* lResult;

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
