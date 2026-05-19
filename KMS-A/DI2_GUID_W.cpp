
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_GUID_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Output.h>

#include <KMS/DI2/GUID.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void GUID::Code_ASCII(const void* aData, DI2::Output* aOutput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aOutput);

            auto lData = reinterpret_cast<const ::GUID*>(aData);

            char lStr[48];

            sprintf_s(lStr,
                "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
                lData->Data1,
                lData->Data2,
                lData->Data3,
                lData->Data4[0],
                lData->Data4[1],
                lData->Data4[2],
                lData->Data4[3],
                lData->Data4[4],
                lData->Data4[5],
                lData->Data4[6],
                lData->Data4[7]);

            aOutput->Token_Append(DI2::TokenType::QUOTED, lStr);
        }

        void GUID::Code_JSON(const void* aData, DI2::Output* aOutput) const
        {
            // TODO JSON
            assert(false);
        }

        // Operator =
        //
        // "{00000000-0000-0000-0000-000000000000}"
        // op "{00000000-0000-0000-0000-000000000000}"
        void GUID::Decode_ASCII(void* aData, KMS::DI2::Input* aInput) const
        {
            assert(nullptr != aData);
            assert(nullptr != aInput);

            auto lTT = aInput->Token_Next(DI2::TokenType::OPERATOR | DI2::TokenType::QUOTED);
            if (DI2::TokenType::OPERATOR == lTT)
            {
                auto lOP = aInput->Token_GetOperator();
                KMS_EXCEPTION_ASSERT(DI2::Operator::ASSIGN == lOP, RESULT_INVALID_VALUE, "Invalid operator", "");

                aInput->Token_Next(DI2::TokenType::QUOTED);
            }

            char lStr[48];

            aInput->Token_GetText(lStr, sizeof(lStr));

            auto lData = reinterpret_cast<::GUID*>(aData);

            *lData = Convert::ToGUID(lStr);
        }

        void GUID::Decode_JSON(void* aData, KMS::DI2::Input* aInput) const
        {
            // TODO JSON
            assert(false);
        }

        const GUID TYPE_GUID;

    }
}
