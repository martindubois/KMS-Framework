
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Version_DI2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Output.h>
#include <KMS/String.h>

#include <KMS/String_DI2.h>

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

static void StringList_ASCII_Decode_ASCII(KMS::StringList_ASCII* aData, KMS::DI2::Input* aInput, KMS::DI2::TokenType aTokenTypes);

namespace KMS
{

    // { "String0", "String1" }
    void StringList_ASCII_Type::Code_ASCII(const void* aData, DI2::Output* aOutput) const
    {
        assert(nullptr != aData);
        assert(nullptr != aOutput);

        auto lData = reinterpret_cast<const StringList_ASCII*>(aData);

        auto lIt = lData->begin();

        aOutput->Char_Append('{');

        if (lData->end() != lIt)
        {
            aOutput->Token_Append(DI2::TokenType::QUOTED, lIt->c_str());
            lIt++;

            while (lData->end() != lIt)
            {
                aOutput->Char_Append(',');
                aOutput->Token_Append(DI2::TokenType::QUOTED, lIt->c_str());
            }
        }

        aOutput->Char_Append('}');
    }

    void StringList_ASCII_Type::Code_JSON(const void* aData, DI2::Output* aOutput) const
    {
        // TODO
    }

    // { "String0", "String1" }
    // "String0"
    // = { "String0", "String1" }
    // = "String0"
    // += "String0"
    // -= "String0"
    void StringList_ASCII_Type::Decode_ASCII(void* aData, DI2::Input* aInput) const
    {
        auto lData = reinterpret_cast<StringList_ASCII*>(aData);

        StringList_ASCII_Decode_ASCII(lData, aInput, DI2::TokenType::OPERATOR | DI2::TokenType::QUOTED | DI2::TokenType::STRING);
    }

    void StringList_ASCII_Type::Decode_JSON(void* aData, DI2::Input* aInput) const
    {
        // TODO
    }

}

using namespace KMS;

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

static void StringList_ASCII_Decode_ASCII(StringList_ASCII* aData, DI2::Input* aInput, DI2::TokenType aTokenTypes)
{
    assert(nullptr != aData);
    assert(nullptr != aInput);

    if (aInput->Char_Next_Try('{'))
    {
        // TODO
    }
    else
    {
        StringList_ASCII::iterator lIt;
        char lString[LINE_LENGTH];

        auto lTT = aInput->Token_Next(aTokenTypes);
        switch (lTT)
        {
        case DI2::TokenType::OPERATOR:
            DI2::Operator lOp;
            lOp = aInput->Token_GetOperator();
            switch (lOp)
            {
            case DI2::Operator::ADD:
                aInput->Token_Next(DI2::TokenType::QUOTED | DI2::TokenType::STRING);
                aInput->Token_GetText(lString, sizeof(lString));
                aData->push_back(lString);
                break;
            case DI2::Operator::ASSIGN:
                StringList_ASCII_Decode_ASCII(aData, aInput, DI2::TokenType::QUOTED | DI2::TokenType::STRING);
                break;
            case DI2::Operator::SUB:
                aInput->Token_Next(DI2::TokenType::QUOTED | DI2::TokenType::STRING);
                aInput->Token_GetText(lString, sizeof(lString));
                lIt = aData->begin();
                while (aData->end() != lIt)
                {
                    if (0 == strcmp(lString, lIt->c_str()))
                    {
                        aData->erase(lIt);
                        break;
                    }

                    lIt++;
                }
                break;

            default: KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid operator", "");
            }
            break;

        case DI2::TokenType::QUOTED:
        case DI2::TokenType::STRING:
            aInput->Token_GetText(lString, sizeof(lString));
            aData->clear();
            aData->push_back(lString);
            break;
        }
    }
}
