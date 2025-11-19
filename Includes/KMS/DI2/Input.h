
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Input.h
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

        class Input final
        {

        public:

            Input();

            ~Input();

            void Init_String(const char* aString);
            void Init_File  (const char* aFile);

            void Previous();

            char Char_Next();
            void Char_Next(char aChar);
            char Char_Next(const char* aChars);
            bool Char_Next_Try(char aChar);

            TokenType    Token_Next(TokenType aTypes);
            char         Token_GetChar() const;
            double       Token_GetFloat() const;
            int64_t      Token_GetInt() const;
            Operator     Token_GetOperator() const;
            unsigned int Token_GetSize() const;
            void         Token_GetText(char* aOut, unsigned int aOutSize_byte) const;
            uint64_t     Token_GetUInt() const;

        private:

            void SkipBlank();

            bool         mDelete;
            unsigned int mIndex;
            unsigned int mPrevious;
            const char * mString;
            char         mValue[LINE_LENGTH];

        };

        void Decode_ASCII          (void* aData, const IType* aType, Input* aInput);
        void Decode_ASCII_Arguments(void* aData, const IType* aType, int aCount, const char** aVector);
        void Decode_ASCII_File     (void* aData, const IType* aType, const char* aFile);
        void Decode_ASCII_String   (void* aData, const IType* aType, const char* aString);
        void Decode_JSON           (void* aData, const IType* aType, Input* aInput);
        void Decode_JSON_String    (void* aData, const IType* aType, const char* aString);

    }
}
