
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Input.cpp

// CODE REVIEW
// TEST COVERAGE 2026-01-24 Martin Dubois

#include "Component.h"

// ===== C++ ================================================================
#include <fstream>

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>
#include <KMS/Environment.h>

#include <KMS/DI2/Input.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static bool ReadArgumentFile(void* aData, const KMS::DI2::IType* aType, const char* aString);

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Input::Input()
            : mDelete(false)
            , mIndex(0)
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

        bool Input::IsConsummed()
        {
            assert(nullptr != mString);

            SkipBlank();

            return ('\0' == mString[mIndex]);
        }

        void Input::Next(const std::regex& aRegex)
        {
            mStringR = mString + mIndex;

            std::smatch lMatch;

            auto lRet = std::regex_search(mStringR, lMatch, aRegex);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_FORMAT, "Invalid format", "");

            mIndex += static_cast<unsigned int>(lMatch[0].length());
        }

        void Input::Next(const std::regex& aRegex, std::smatch& aMatch)
        {
            mStringR = mString + mIndex;

            auto lRet = std::regex_search(mStringR, aMatch, aRegex);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_FORMAT, "Invalid format", "");

            mIndex += static_cast<unsigned int>(aMatch[0].length());
        }

        bool Input::Next_Try(const std::regex& aRegex)
        {
            mStringR = mString + mIndex;

            std::smatch lMatch;

            auto lResult = std::regex_search(mStringR, lMatch, aRegex);
            if (lResult)
            {
                mIndex += static_cast<unsigned int>(lMatch[0].length());
            }

            return lResult;
        }

        bool Input::Next_Try(const std::regex& aRegex, std::smatch& aMatch)
        {
            mStringR = mString + mIndex;

            auto lResult = std::regex_search(mStringR, aMatch, aRegex);
            if (lResult)
            {
                mIndex += static_cast<unsigned int>(aMatch[0].length());
            }

            return lResult;
        }

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

        void Decode_ASCII_Env(void* aData, const IType* aType, const char* aVarName)
        {
            try
            {
                char lValue[LINE_LENGTH];

                auto lRet = Env::GetVariableValue(aVarName, lValue, sizeof(lValue));

                auto lPart = lValue;

                for (unsigned int i = 0; i < lRet; i++)
                {
                    if ('\0' == lValue[i])
                    {
                        Decode_ASCII_String(aData, aType, lPart);
                        break;
                    }

                    if (';' == lValue[i])
                    {
                        lValue[i] = '\0';

                        Decode_ASCII_String(aData, aType, lPart);

                        lPart = lValue + i + 1;
                    }
                }
            }
            catch (KMS::Exception)
            {}
        }

        void Decode_ASCII_File(void* aData, const IType* aType, const char* aFile)
        {
            std::ifstream lFile(aFile, std::ios::binary);
            KMS_EXCEPTION_ASSERT(lFile.is_open(), RESULT_OPEN_FAILED, "Cannot open input file", aFile);

            std::string lLine;

            while (getline(lFile, lLine))
            {
                static std::regex REGEX_COMMENT("^\\s*#.*\r?\n?$");
                static std::regex REGEX_EMPTY  ("^\\s*\r?\n?$");
                static std::regex REGEX_STRING ("^\\s*(.+)\\s*\r?\n?$");

                std::smatch lMatch;

                if (std::regex_match(lLine, lMatch, REGEX_STRING))
                {
                    Decode_ASCII_String(aData, aType, lMatch[1].str().c_str());
                }
                else
                {
                    auto lRet = std::regex_match(lLine, lMatch, REGEX_COMMENT) || std::regex_match(lLine, lMatch, REGEX_EMPTY);
                    KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_FORMAT, "Invalid file format", lLine.c_str());
                }
            }
        }

        void Decode_ASCII_String(void* aData, const IType* aType, const char* aString)
        {
            assert(nullptr != aType);

            auto lRet = ReadArgumentFile(aData, aType, aString);
            if (!lRet)
            {
                Input lInput;

                lInput.Init_String(aString);

                aType->Decode_ASCII(aData, &lInput);

                KMS_EXCEPTION_ASSERT(lInput.IsConsummed(), RESULT_INVALID_FORMAT, "Invalid format", aString);
            }
        }

        bool Decode_ASCII_String_Try(void* aData, const IType* aType, const char* aString)
        {
            assert(nullptr != aType);

            auto lResult = ReadArgumentFile(aData, aType, aString);
            if (!lResult)
            {
                Input lInput;

                lInput.Init_String(aString);

                try
                {
                    aType->Decode_ASCII(aData, &lInput);

                    lResult = lInput.IsConsummed();
                }
                catch (KMS::Exception eE)
                {
                    if (RESULT_INVALID_NAME != eE.GetCode())
                    {
                        throw eE;
                    }
                }
            }

            return lResult;
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

bool ReadArgumentFile(void* aData, const KMS::DI2::IType* aType, const char* aString)
{
    static const std::regex REGEX("^@\\s*(.+)\\s*$");

    assert(nullptr != aString);

    std::smatch lMatch;
    std::string lString(aString);

    bool lResult = std::regex_match(lString, lMatch, REGEX);
    if (lResult)
    {
        DI2::Decode_ASCII_File(aData, aType, lMatch[1].str().c_str());
    }

    return lResult;
}
