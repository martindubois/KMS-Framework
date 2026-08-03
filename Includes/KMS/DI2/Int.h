
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Int.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Convert.h>
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

        extern const std::regex Int_REGEX_OP_VALUE_C2;
        extern const std::regex Int_REGEX_VALUE_C1;

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
            assert(false);
        }

        // TODO  Use regex

        // Operators += -= *= /= |= &= ^=
        //
        // Value
        // op Value
        template <typename T>
        void Int<T>::Decode_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            std::smatch lMatch;
            int64_t     lValue;

            if (aInput->Next_Try(Int_REGEX_VALUE_C1, lMatch))
            {
                lValue = Convert::ToInt64(lMatch[1].str().c_str());
            }
            else
            {
                aInput->Next(Int_REGEX_OP_VALUE_C2, lMatch);

                Enum<Operator, Operator_SYMBOLS> lOp(lMatch[1].str().c_str());

                lValue = Convert::ToInt32(lMatch[2].str().c_str());

                lValue = Operator_Eval(*reinterpret_cast<T*>(aData), lOp, lValue);
            }

            *reinterpret_cast<T*>(aData) = static_cast<T>(lValue);

            KMS_EXCEPTION_ASSERT(*reinterpret_cast<T*>(aData) == lValue, RESULT_INVALID_VALUE, "Integer value too large", "");
        }

        // Value
        template <typename T>
        void Int<T>::Decode_JSON(void* aData, Input* aInput) const
        {
            assert(false);
        }

    }
}
