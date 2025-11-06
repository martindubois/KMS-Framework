
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Struct.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Includes ===========================================================
#include <KMS/DI2/Sized.h>

namespace KMS
{
    namespace DI2
    {

        class Struct : public Sized
        {

        public:

            Struct(unsigned int aSize_byte);

            void AddField(const char* aName, unsigned int aOffset_byte, const IType* aType);

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext = nullptr) const override;
            virtual void Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext = nullptr) const override;
            virtual void Decode_ASCII(void* aData, const char* aIn, const char** aNext = nullptr) const override;
            virtual void Decode_JSON(void* aData, const char* aIn, const char** aNext = nullptr) const override;

        private:

            class Field
            {

            public:

                Field(unsigned int aOffset_byte, const IType* aType);

            private:

                unsigned int mOffset_byte;
                const IType* mType;

            };

            typedef std::map<std::string, Field> FieldList;

            FieldList mFields;

        };

    }
}