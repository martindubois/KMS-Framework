
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/BitField.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

namespace KMS
{
    namespace DI2
    {

        class BitField : public IType
        {

        public:

            BitField(const IType* aType);

            void AddField(const char* aName, unsigned int aBits);

            // ===== IType ==================================================
            virtual unsigned int GetSize() const override;
            virtual void Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext = nullptr) const override;
            virtual void Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext = nullptr) const override;
            virtual void Decode_ASCII(void* aData, const char* aIn, const char** aNext = nullptr) const override;
            virtual void Decode_JSON(void* aData, const char* aIn, const char** aNext = nullptr) const override;

        private:

            typedef std::map<std::string, unsigned int> FieldList;

            FieldList mFields;

            const IType* mType;

        };

    }
}
