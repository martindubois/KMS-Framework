
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2026 KMS
/// \file      Includes/KMS/DI2/Enum.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Input.h>
#include <KMS/DI2/IType.h>
#include <KMS/DI2/Output.h>
#include <KMS/Enum.h>

namespace KMS
{
    namespace DI2
    {

        template <typename T, const char** N>
        class Enum : public IType
        {

        public:

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        };

        extern const std::regex Enum_REGEX_VALUE_NAME_C1;

        // Public
        // //////////////////////////////////////////////////////////////////

        // ValueName
        template <typename T, const char** N>
        void Enum<T, N>::Code_ASCII(const void* aData, Output* aOutput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aOutput);

            KMS::Enum<T, N> lValue(*reinterpret_cast<const T*>(aData));

            aOutput->Token_Append(TokenType::STRING, lValue.GetName());
        }

        template <typename T, const char** N>
        void Enum<T, N>::Code_JSON(const void* aData, Output* aOutput) const {}

        // ValueName
        // = ValueName
        template <typename T, const char** N>
        void Enum<T, N>::Decode_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            std::smatch lMatch;

            aInput->Next(Enum_REGEX_VALUE_NAME_C1, lMatch);

            KMS::Enum<T, N> lValue(lMatch[1].str().c_str());

            *reinterpret_cast<T*>(aData) = lValue;
        }

        template <typename T, const char** N>
        void Enum<T, N>::Decode_JSON(void* aData, Input* aInput) const {}

    }
}
