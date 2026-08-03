
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Version_DI2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Operator.h>
#include <KMS/DI2/Output.h>
#include <KMS/String.h>

#include <KMS/String_DI2.h>

// ===== Local ==============================================================
#include "DI2_Regex.h"

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

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
        // TODO  JSON
    }

    // { "String0", "String1" }
    // "String0"
    // = { "String0", "String1" }
    // = "String0"
    // += "String0"
    // -= "String0"
    void StringList_ASCII_Type::Decode_ASCII(void* aData, DI2::Input* aInput) const
    {
        static const std::regex REGEX_ELEMENT_END (DI2_Regex_BEGIN DI2_Regex_SPACE "," DI2_Regex_SPACE);
        static const std::regex REGEX_GROUP_BEGIN (DI2_Regex_GROUP_BEGIN);
        static const std::regex REGEX_GROUP_END   (DI2_Regex_GROUP_END);
        static const std::regex REGEX_OP_STRING_C2(DI2_Regex_BEGIN DI2_Regex_OP_C1      DI2_Regex_SPACE "\"(.*)\"" DI2_Regex_SPACE);
        static const std::regex REGEX_STRING_C1   (DI2_Regex_BEGIN DI2_Regex_ASSIGN_OPT DI2_Regex_SPACE "\"(.*)\"" DI2_Regex_SPACE);

        auto lData = reinterpret_cast<StringList_ASCII*>(aData);

        std::smatch lMatch;

        if (aInput->Next_Try(REGEX_STRING_C1, lMatch))
        {
            lData->clear();
            lData->push_back(lMatch[1].str().c_str());
        }
        else if (aInput->Next_Try(REGEX_OP_STRING_C2, lMatch))
        {
            Enum<DI2::Operator, DI2::Operator_SYMBOLS> lOp(lMatch[1].str().c_str());

            switch (lOp)
            {
            case DI2::Operator::ADD: lData->push_back(lMatch[2].str().c_str()); break;
            case DI2::Operator::SUB: lData->remove   (lMatch[2].str().c_str()); break;

            default: KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid string list operator", lMatch[1].str().c_str());
            }
        }
        else
        {
            aInput->Next(REGEX_GROUP_BEGIN);

            lData->clear();

            if (!aInput->Next_Try(REGEX_GROUP_END))
            {
                do
                {
                    aInput->Next(REGEX_STRING_C1, lMatch);

                    lData->push_back(lMatch[1].str().c_str());
                }
                while (aInput->Next_Try(REGEX_ELEMENT_END));

                aInput->Next(REGEX_GROUP_END);
            }
        }
    }

    void StringList_ASCII_Type::Decode_JSON(void* aData, DI2::Input* aInput) const
    {
        // TODO  JSON
    }

}
