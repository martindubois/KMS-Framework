
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Array.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI2/Container.h>
#include <KMS/DI2/Input.h>
#include <KMS/DI2/IType.h>
#include <KMS/DI2/Output.h>

namespace KMS
{
    namespace DI2
    {

        template <typename T, unsigned int N>
        class Array : public Container, public IType
        {

        public:

            Array(const IType* aElementType);

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        };

        extern const std::regex Array_REGEX_GROUP_BEGIN;
        extern const std::regex Array_REGEX_GROUP_END;

        extern const std::regex Array_REGEX_INDEX_DEC_C1;
        extern const std::regex Array_REGEX_INDEX_HEX_C1;

        extern const std::regex Array_REGEX_VALUE_END;

        template <typename T, unsigned int N>
        Array<T, N>::Array(const IType* aElementType) : Container(aElementType) {}

        // { Value0; Value1; }
        template <typename T, unsigned int N>
        void Array<T, N>::Code_ASCII(const void* aData, Output* aOutput) const
        {
            assert(nullptr != aOutput);

            aOutput->Char_Append('{');

            CodeElement_ASCII(aData, aOutput);

            auto lData = reinterpret_cast<const uint8_t*>(aData);

            for (unsigned int i = 1; i < N; i++)
            {
                aOutput->Char_Append(',');

                CodeElement_ASCII(lData + sizeof(T) * i, aOutput);
            }

            aOutput->Char_Append('}');
        }

        // [ Value0, Value1 ]
        template <typename T, unsigned int N>
        void Array<T, N>::Code_JSON(const void* aData, Output* aOutput) const
        {
            assert(false);
        }

        // [0] op Value0
        // { Value0, Value1 }
        // = { Value0, Value1 }
        template <typename T, unsigned int N>
        void Array<T, N>::Decode_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            auto lData = reinterpret_cast<T*>(aData);

            std::smatch lMatch;

            if (   aInput->Next_Try(Array_REGEX_INDEX_DEC_C1, lMatch)
                || aInput->Next_Try(Array_REGEX_INDEX_HEX_C1, lMatch))
            {
                auto lIndex = KMS::Convert::ToUInt32(lMatch[1].str().c_str());

                KMS_EXCEPTION_ASSERT(N > lIndex, RESULT_INVALID_INDEX, "Invalid index", "");

                DecodeElement_ASCII(lData + lIndex, aInput);
            }
            else
            {
                aInput->Next(Array_REGEX_GROUP_BEGIN);

                if (!aInput->Next_Try(Array_REGEX_GROUP_END))
                {
                    uint32_t lIndex = 0;

                    do
                    {
                        KMS_EXCEPTION_ASSERT(N > lIndex, RESULT_INVALID_INDEX, "Invalid index", "");

                        DecodeElement_ASCII(lData + lIndex, aInput);

                        lIndex++;
                    }
                    while (aInput->Next_Try(Array_REGEX_VALUE_END));

                    aInput->Next(Array_REGEX_GROUP_END);
                }
            }
        }

        // [ Value0, Value1 ]
        template <typename T, unsigned int N>
        void Array<T, N>::Decode_JSON(void* aData, Input* aInput) const
        {
            assert(false);
        }

    }
}
