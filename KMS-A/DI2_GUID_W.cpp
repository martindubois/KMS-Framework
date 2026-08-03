
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

// ===== Local ==============================================================
#include "DI2_Regex.h"

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
        // {00000000-0000-0000-0000-000000000000}
        // "{00000000-0000-0000-0000-000000000000}"
        // op {00000000-0000-0000-0000-000000000000}
        // op "{00000000-0000-0000-0000-000000000000}"
        void GUID::Decode_ASCII(void* aData, KMS::DI2::Input* aInput) const
        {
            static const std::regex REGEX_0_C1(DI2_Regex_BEGIN DI2_Regex_ASSIGN_OPT DI2_Regex_SPACE "(\\{[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}\\})"     DI2_Regex_SPACE);
            static const std::regex REGEX_1_C1(DI2_Regex_BEGIN DI2_Regex_ASSIGN_OPT DI2_Regex_SPACE "\"(\\{[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}\\})\"" DI2_Regex_SPACE);

            assert(nullptr != aData);
            assert(nullptr != aInput);

            std::smatch lMatch;

            bool lRet = aInput->Next_Try(REGEX_0_C1, lMatch)
                ||      aInput->Next_Try(REGEX_1_C1, lMatch);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_FORMAT, "Invalid GUID format", "");

            auto lData = reinterpret_cast<::GUID*>(aData);

            *lData = Convert::ToGUID(lMatch[1].str().c_str());
        }

        void GUID::Decode_JSON(void* aData, KMS::DI2::Input* aInput) const
        {
            // TODO JSON
            assert(false);
        }

        const GUID TYPE_GUID;

    }
}
