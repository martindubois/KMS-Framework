
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Input.cpp

// CODE REVIEW
// TEST COVERAGE 2026-01-24 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

#include <KMS/DI2/Input.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define FORMAT_INT           "%lld"
#define FORMAT_INT_STR       "%[0-9-]"
#define FORMAT_FLOAT         "%lf"
#define FORMAT_FLOAT_STR     "%[0-9.]"
#define FORMAT_NAME_STR      "%[0-9A-Z_a-z]"
#define FORMAT_OPERATOR_STR  "%[&*+-/=|^]"
#define FORMAT_QUOTED_STR    "\"%[^\"]\""
#define FORMAT_STRING_STR    "%[^\n\r\t]"
#define FORMAT_UINT          "%llu"
#define FORMAT_UINT_STR      "%[0-9]"
#define FORMAT_UINT_HEX      "0x%llx"
#define FORMAT_UINT_HEX_STR  "%[0-9A-Fa-fx]"

#define INVALID_INDEX (0xffffffff)

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Input::Input()
            : mDelete(false)
            , mIndex(0)
            , mPrevious(INVALID_INDEX)
            , mString(nullptr)
        {
            memset(mValue, 0, sizeof(mValue));
        }

        Input::~Input()
        {
            if (mDelete)
            {
                assert(nullptr != mString);

                delete[] mString;
            }
        }

        void Input::Init_File(const char* aFile)
        {
            assert(nullptr != aFile);

            assert(!mDelete);
            assert(0 == mIndex);
            assert(nullptr == mString);

            FILE* lFile = nullptr;

            auto lRet = fopen_s(&lFile, aFile, "rb");
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_OPEN_FAILED, "Cannot open input file", aFile);

            assert(nullptr != lFile);

            lRet = fseek(lFile, 0, SEEK_END);
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_OPERATION_FAILED, "Cannot retrieve file size (NOT TESTED)", aFile);

            auto lSize_byte = ftell(lFile);
            if (0 < lSize_byte)
            {
                lRet = fseek(lFile, 0, SEEK_SET);
                assert(0 == lRet);

                mDelete = true;
                mIndex  = 0;
                mString = new char[lSize_byte];

                auto lRet_byte = fread(const_cast<char*>(mString), sizeof(char), lSize_byte, lFile);
                KMS_EXCEPTION_ASSERT(lSize_byte == lRet_byte, RESULT_READ_FAILED, "Cannot read input file (NOT TESTED)", aFile);
            }

            lRet = fclose(lFile);
            assert(0 == lRet);
        }

        void Input::Init_String(const char* aString)
        {
            assert(nullptr != aString);

            assert(!mDelete);
            assert(0 == mIndex);
            assert(nullptr == mString);

            mDelete = false;
            mIndex  = 0;
            mString = aString;
        }

        void Input::Previous()
        {
            assert(mIndex > mPrevious);

            mIndex    = mPrevious;
            mPrevious = INVALID_INDEX;
        }

        void Input::Char_Next(char aChar)
        {
            SkipBlank();

            KMS_EXCEPTION_ASSERT(mString[mIndex] == aChar, RESULT_INVALID_FORMAT, "Invalid format", "");

            mPrevious = mIndex;
            mIndex++;
        }

        char Input::Char_Next(const char* aChars)
        {
            assert(nullptr != aChars);

            SkipBlank();

            // TODO Handle the end of string condition

            auto lPtr = strchr(aChars, mString[mIndex]);
            KMS_EXCEPTION_ASSERT(nullptr != lPtr, RESULT_INVALID_FORMAT, "Invalid format", "");

            mPrevious = mIndex;
            mIndex++;

            return *lPtr;
        }

        bool Input::Char_Next_Try(char aChar)
        {
            SkipBlank();

            bool lResult = (mString[mIndex] == aChar);
            if (lResult)
            {
                mPrevious = mIndex;
                mIndex++;
            }

            return lResult;
        }

        double Input::Token_GetFloat() const
        {
            double lResult;

            auto lRet = sscanf_s(mValue, FORMAT_FLOAT, &lResult);
            KMS_EXCEPTION_ASSERT(1 == lRet, RESULT_INVALID_FORMAT, "Invalid floating point value", mValue);

            return lResult;
        }

        int64_t Input::Token_GetInt() const
        {
            int64_t lResult;

            auto lRet = sscanf_s(mValue, FORMAT_INT, &lResult);
            KMS_EXCEPTION_ASSERT(1 == lRet, RESULT_INVALID_FORMAT, "Invalid integer value", mValue);

            return lResult;
        }

        Operator Input::Token_GetOperator() const
        {
            Operator lResult;

            if      (0 == strcmp(mValue, "=" )) { lResult = Operator::ASSIGN; }
            else if (0 == strcmp(mValue, "+=")) { lResult = Operator::ADD; }
            else if (0 == strcmp(mValue, "&=")) { lResult = Operator::AND; }
            else if (0 == strcmp(mValue, "/=")) { lResult = Operator::DIV; }
            else if (0 == strcmp(mValue, "*=")) { lResult = Operator::MULT; }
            else if (0 == strcmp(mValue, "|=")) { lResult = Operator::OR; }
            else if (0 == strcmp(mValue, "-=")) { lResult = Operator::SUB; }
            else if (0 == strcmp(mValue, "^=")) { lResult = Operator::XOR; }
            else
            {
                KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid operator", mValue);
            }

            return lResult;
        }

        void Input::Token_GetText(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);

            memset(aOut, 0, aOutSize_byte);

            auto lLen = strlen(mValue);

            KMS_EXCEPTION_ASSERT(aOutSize_byte > lLen, RESULT_OUTPUT_TOO_SHORT, "String is too long", mValue);

            memcpy(aOut, mValue, lLen);
        }

        uint64_t Input::Token_GetUInt() const
        {
            uint64_t lResult;

            if (1 != sscanf_s(mValue, FORMAT_UINT_HEX, &lResult))
            {
                auto lRet = sscanf_s(mValue, FORMAT_UINT, &lResult);
                KMS_EXCEPTION_ASSERT(1 == lRet, RESULT_INVALID_FORMAT, "Invalid unsigned integer format", mValue);
            }

            return lResult;
        }

        #define PARSE_0(T,F)                                                                                        \
            if (IsTokenTypePresent(aTypes, (T)) && (1 == sscanf_s(mString + mIndex, (F), mValue SizeInfo(mValue)))) \
            {                                                                                                       \
                lResult = (T); goto End;                                                                            \
            }

        TokenType Input::Token_Next(TokenType aTypes)
        {
            SkipBlank();

            memset(mValue, 0, sizeof(mValue));

            if (   IsTokenTypePresent(aTypes, TokenType::END)
                && ('\0' == mString[mIndex]))
            {
                return TokenType::END;
            }

            KMS_EXCEPTION_ASSERT('\0' != mString[mIndex], RESULT_INVALID_FORMAT, "Unexpected end", "");

            TokenType lResult;

            PARSE_0(TokenType::FLOAT   , FORMAT_FLOAT_STR   );
            PARSE_0(TokenType::INT     , FORMAT_INT_STR     );
            PARSE_0(TokenType::OPERATOR, FORMAT_OPERATOR_STR);
            PARSE_0(TokenType::QUOTED  , FORMAT_QUOTED_STR  );
            PARSE_0(TokenType::UINT_HEX, FORMAT_UINT_HEX_STR);

            PARSE_0(TokenType::UINT    , FORMAT_UINT_STR    );

            PARSE_0(TokenType::NAME    , FORMAT_NAME_STR    );

            PARSE_0(TokenType::STRING  , FORMAT_STRING_STR  );

            KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid format", "");

        End:
            mPrevious = mIndex;
            mIndex += static_cast<unsigned int>(strlen(mValue));
            return lResult;
        }

        #define PARSE_1(T,F)                                                       \
            case (T):                                                              \
                if (1 == sscanf_s(mString + mIndex, (F), mValue SizeInfo(mValue))) \
                {                                                                  \
                    goto End;                                                      \
                }                                                                  \
                break

        // Private
        // //////////////////////////////////////////////////////////////////

        void Input::SkipBlank()
        {
            for (;;)
            {
                auto lChar = mString[mIndex];

                if (('\n' != lChar) && ('\r' != lChar) && ('\t' != lChar) && (' ' != lChar))
                {
                    break;
                }

                mIndex++;
            }
        }

        // Functions
        // //////////////////////////////////////////////////////////////////

        void Decode_ASCII_Arguments(void* aData, const IType* aType, int aCount, const char** aVector)
        {
            assert(nullptr != aVector);

            for (int i = 0; i < aCount; i++)
            {
                Decode_ASCII_String(aData, aType, aVector[i]);
            }
        }

        void Decode_ASCII_Arguments(void* aData, const IType* aType, ArgList* aArgList)
        {
            assert(nullptr != aArgList);

            auto lCount = aArgList->GetCount();

            for (unsigned int i = 0; i < lCount; i++)
            {
                auto lArg = aArgList->GetArgument(i);
                assert(nullptr != lArg);

                if (Decode_ASCII_String_Try(aData, aType, lArg))
                {
                    aArgList->IncUseCount(i);
                }
            }
        }

        void Decode_ASCII_String(void* aData, const IType* aType, const char* aString)
        {
            assert(nullptr != aType);

            Input lInput;

            lInput.Init_String(aString);

            aType->Decode_ASCII(aData, &lInput);
        }

        bool Decode_ASCII_String_Try(void* aData, const IType* aType, const char* aString)
        {
            assert(nullptr != aType);

            Input lInput;

            lInput.Init_String(aString);

            bool lResult = false;

            try
            {
                aType->Decode_ASCII(aData, &lInput);

                lResult = true;
            }
            catch (KMS::Exception eE)
            {
                if (RESULT_INVALID_NAME != eE.GetCode())
                {
                    throw eE;
                }
            }

            return lResult;
        }

    }
}
