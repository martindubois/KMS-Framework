
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Output.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

#include <KMS/DI2/Output.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Output::Output() : mDelete(false), mIndex(0), mSize_byte(0), mString(nullptr) {}

        Output::~Output()
        {
            if (mDelete)
            {
                delete [] mString;
            }
        }

        void Output::Init(unsigned int aSize_byte, char* aBuffer)
        {
            mIndex = 0;
            mSize_byte = aSize_byte;

            mDelete = (nullptr == aBuffer);
            if (mDelete)
            {
                mString = new char[mSize_byte];
                mDelete = true;
            }
            else
            {
                mString = aBuffer;
            }

            memset(mString, 0, mSize_byte);
        }

        void Output::Char_Append(char aChar)
        {
            auto lNewIndex = mIndex + 1;

            KMS_EXCEPTION_ASSERT(mSize_byte > lNewIndex, RESULT_OUTPUT_TOO_SHORT, "Output buffer is too short", "");

            mString[mIndex] = aChar;

            mIndex = lNewIndex;
        }

        void Output::Token_Append(double aFloat)
        {
            char lString[64];

            sprintf_s(lString, "%f", aFloat);

            Token_Append(TokenType::STRING, lString);
        }

        void Output::Token_Append(int64_t aInt)
        {
            char lString[64];

            sprintf_s(lString, "%lld", aInt);

            Token_Append(TokenType::STRING, lString);
        }

        void Output::Token_Append(TokenType aType, char aChar)
        {
            char lString[2];

            sprintf_s(lString, "%c", aChar);

            Token_Append(aType, lString);
        }

        void Output::Token_Append(TokenType aType, const char* aValue)
        {
            char lBuffer[LINE_LENGTH];
            auto lValue = aValue;

            switch (aType)
            {
            case TokenType::NAME:
            case TokenType::OPERATOR:
            case TokenType::STRING:
                break;

            case TokenType::QUOTED:
                sprintf_s(lBuffer, "\"%s\"", aValue);
                lValue = lBuffer;
                break;

            default: assert(false);
            }

            auto lLen = static_cast<unsigned int>(strlen(lValue));
            auto lNewIndex = mIndex + lLen;

            KMS_EXCEPTION_ASSERT(mSize_byte > lNewIndex, RESULT_OUTPUT_TOO_SHORT, "Output buffer is too short", aValue);

            memcpy(mString + mIndex, lValue, lLen);

            mIndex = lNewIndex;
        }

        void Code_ASCII_String(void* aData, const IType* aType, unsigned int aSize_byte, char* aBuffer)
        {
            assert(nullptr != aType);
            assert(nullptr != aBuffer);

            Output lOutput;

            lOutput.Init(aSize_byte, aBuffer);

            aType->Code_ASCII(aData, &lOutput);
        }

    }
}
