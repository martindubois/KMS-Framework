
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/UInt.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Output.h>
#include <KMS/DI2/IType.h>
#include <KMS/Enum.h>

namespace KMS
{
    namespace DI2
    {

        template <typename T>
        class UInt final : public IType
        {

        public:

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        };

        extern const UInt<uint64_t> TYPE_UINT64;
        extern const UInt<uint32_t> TYPE_UINT32;
        extern const UInt<uint16_t> TYPE_UINT16;
        extern const UInt<uint8_t > TYPE_UINT8;

        extern const std::regex UInt_REGEX_OP_VALUE_DEC_C2;
        extern const std::regex UInt_REGEX_OP_VALUE_HEX_C2;
        extern const std::regex UInt_REGEX_VALUE_DEC_C1;
        extern const std::regex UInt_REGEX_VALUE_HEX_C1;

        // Value
        template <typename T>
        void UInt<T>::Code_ASCII(const void* aData, Output* aOutput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aOutput);

            int64_t lValue = *reinterpret_cast<const T*>(aData);

            aOutput->Token_Append(lValue);
        }

        // Value
        template <typename T>
        void UInt<T>::Code_JSON(const void* aData, Output* aOutput) const
        {
            // TODO  JSON
        }

        // Operators = += -= *= /= |= &= ^=
        //
        // Value
        // op Value
        template <typename T>
        void UInt<T>::Decode_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            std::smatch lMatch;
            uint64_t    lValue;

            if (  aInput->Next_Try(UInt_REGEX_VALUE_HEX_C1, lMatch)
               || aInput->Next_Try(UInt_REGEX_VALUE_DEC_C1, lMatch))
            {
                lValue = Convert::ToUInt64(lMatch[1].str().c_str());
            }
            else
            {
                auto lRet = aInput->Next_Try(UInt_REGEX_OP_VALUE_HEX_C2, lMatch)
                    ||      aInput->Next_Try(UInt_REGEX_OP_VALUE_DEC_C2, lMatch);
                KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_FORMAT, "Invalid UInt format", "");

                Enum<Operator, Operator_SYMBOLS> lOp(lMatch[1].str().c_str());

                lValue = Convert::ToUInt64(lMatch[2].str().c_str());

                lValue = Operator_Eval(*reinterpret_cast<T*>(aData), lOp, lValue);
            }

            *reinterpret_cast<T*>(aData) = static_cast<T>(lValue);

            KMS_EXCEPTION_ASSERT(*reinterpret_cast<T*>(aData) == lValue, RESULT_INVALID_VALUE, "Integer value too large", "");
        }

        // Value
        template <typename T>
        void UInt<T>::Decode_JSON(void* aData, Input* aInput) const
        {
            // TODO  JSON
        }

    }
}
