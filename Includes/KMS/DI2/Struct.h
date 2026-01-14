
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Struct.h
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

        typedef struct
        {
            const char * mName;
            unsigned int mOffset_byte;
            const IType* mType;
        }
        Struct_Field;

        template<const Struct_Field* F>
        class Struct : public IType
        {

        public:

            Struct();

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        private:

            void DecodeField_ASCII(void* aData, Input* aInput) const;

            const Struct_Field* FindField(const char* aName) const;

        };

        template <const Struct_Field* F>
        Struct<F>::Struct() {}

        // { Field0 = Value0; Field1 = Value1; }
        template<const Struct_Field* F>
        void Struct<F>::Code_ASCII(const void* aData, Output* aOutput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aOutput);

            auto lData = reinterpret_cast<const uint8_t*>(aData);

            unsigned int lIndex = 0;

            aOutput->Char_Append('{');
            
            for (;;)
            {
                auto lField = F + lIndex;
                if ((nullptr == lField->mName) || (nullptr == lField->mType))
                {
                    break;
                }

                aOutput->Token_Append(TokenType::NAME, lField->mName);
                aOutput->Char_Append('=');

                lField->mType->Code_ASCII(lData + lField->mOffset_byte, aOutput);

                aOutput->Char_Append(';');

                lIndex++;
            }

            aOutput->Char_Append('}');
        }

        template<const Struct_Field* F>
        void Struct<F>::Code_JSON(const void* aData, Output* aOutput) const
        {
            // TODO
        }

        // Field0 op Value0
        // .Field0 op Value0
        // { Field0 op Value0; Field1 op Value0; }
        // = { Field0 op Value0; Field1 op Value0; }
        template<const Struct_Field* F>
        void Struct<F>::Decode_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aInput);

            if (aInput->Char_Next_Try('='))
            {
                aInput->Char_Next('{');

                while (!aInput->Char_Next_Try('}'))
                {
                    DecodeField_ASCII(aData, aInput);

                    aInput->Char_Next(';');
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
            else
            {
                aInput->Char_Next_Try('.');

                DecodeField_ASCII(aData, aInput);
            }
        }

        template<const Struct_Field* F>
        void Struct<F>::Decode_JSON(void* aData, Input* aInput) const
        {
            // TODO
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        template<const Struct_Field* F>
        void Struct<F>::DecodeField_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            auto lData = reinterpret_cast<uint8_t*>(aData);

            char lName[NAME_LENGTH];

            aInput->Token_Next(TokenType::NAME);
            aInput->Token_GetText(lName, sizeof(lName));

            auto lField = FindField(lName);

            lField->mType->Decode_ASCII(lData + lField->mOffset_byte, aInput);
        }

        template<const Struct_Field* F>
        const Struct_Field* Struct<F>::FindField(const char* aName) const
        {
            assert(nullptr != aName);

            unsigned int lIndex = 0;

            const Struct_Field* lResult = nullptr;

            for (;;)
            {
                lResult = F + lIndex;

                KMS_EXCEPTION_ASSERT((nullptr != lResult->mName) && (nullptr != lResult->mType), RESULT_INVALID_NAME, "Invalid field name", aName);

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
