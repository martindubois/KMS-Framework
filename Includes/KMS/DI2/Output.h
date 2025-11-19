
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Output.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Operator.h>
#include <KMS/DI2/TokenType.h>

namespace KMS
{
    namespace DI2
    {

        class IType;

        class Output
        {

        public:

            Output();

            ~Output();

            void Init(unsigned int aSize_byte = 8129, char* aBuffer = nullptr);

            unsigned int GetSize() const;

            unsigned int GetUsedSize() const;

            char* GetString() const;

            void SaveToFile(const char* aFile) const;

            void Char_Append(char aChar);

            void Token_Append(double aFloat);
            void Token_Append(int64_t aInt);
            void Token_Append(uint64_t aUInt);
            void Token_Append(Operator* aOperator);
            void Token_Append(TokenType aType, char aChar);
            void Token_Append(TokenType aType, const char* aValue);

        private:

            bool         mDelete;
            unsigned int mIndex;
            unsigned int mSize_byte;
            char       * mString;

        };

        void Code_ASCII       (void* aData, const IType* aType, Output* aOutput);
        void Code_ASCII_String(void* aData, const IType* aType, unsigned int aSize_byte, char* aBuffer);
        void Code_JSON        (void* aData, const IType* aType, Output* aOutput);
        void Code_JSON_String (void* aData, const IType* aType, unsigned int aSize_byte, char* aBuffer);

    }
}
