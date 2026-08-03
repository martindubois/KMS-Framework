
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Float.h
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
        class Float final: public IType
        {

        public:

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        };

        extern const Float<double> TYPE_DOUBLE;
        extern const Float<float > TYPE_FLOAT;

        extern const std::regex Float_REGEX_OP_VALUE_C2;
        extern const std::regex Float_REGEX_VALUE_C1;

        // Value
        template <typename T>
        void Float<T>::Code_ASCII(const void* aData, Output* aOutput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aOutput);

            double lValue = *reinterpret_cast<const T*>(aData);

            aOutput->Token_Append(lValue);
        }

        // Value
        template <typename T>
        void Float<T>::Code_JSON(const void* aData, Output* aOutput) const
        {
            assert(false);
        }

        // TODO  Use regex

        // Operators = += -= *= /=
        //
        // Value
        // op Value
        template <typename T>
        void Float<T>::Decode_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            std::smatch lMatch;
            double      lValue;

            if (aInput->Next_Try(Float_REGEX_VALUE_C1, lMatch))
            {
                lValue = Convert::ToDouble(lMatch[1].str().c_str());
            }
            else
            {
                aInput->Next(Float_REGEX_OP_VALUE_C2, lMatch);

                Enum<Operator, Operator_SYMBOLS> lOp(lMatch[1].str().c_str());

                lValue = Convert::ToDouble(lMatch[2].str().c_str());

                lValue = Operator_Eval(*reinterpret_cast<T*>(aData), lOp, lValue);
            }

            *reinterpret_cast<T*>(aData) = static_cast<T>(lValue);
        }

        // Value
        template <typename T>
        void Float<T>::Decode_JSON(void* aData, Input* aInput) const
        {
            assert(false);
        }

    }
}
