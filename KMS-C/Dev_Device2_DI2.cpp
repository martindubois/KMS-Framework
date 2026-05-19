
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device2_DI2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI2/String.h>
#include <KMS/DI2/UInt.h>

#include <KMS/Dev/Device2_DI2.h>

class GUID_Type final : public KMS::DI2::IType
{

public:

    // ===== DI2::IType ======================================================
    virtual void Code_ASCII(const void* aData, KMS::DI2::Output* aOutput) const override;
    virtual void Code_JSON(const void* aData, KMS::DI2::Output* aOutput) const override;
    virtual void Decode_ASCII(void* aData, KMS::DI2::Input* aInput) const override;
    virtual void Decode_JSON(void* aData, KMS::DI2::Input* aInput) const override;

};

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI2::String<PATH_LENGTH> LINK_TYPE;

#ifdef _KMS_WINDOWS_
    static const GUID_Type GUID_TYPE;
#endif

namespace KMS
{
    namespace Dev
    {

        const DI2::Struct_Field Device2_Config_FIELDS[4] =
        {
            { "Link" ,           0, &LINK_TYPE  },
            { "Index", PATH_LENGTH, &DI2::TYPE_UINT8 },

            #ifdef _KMS_LINUX_
                { nullptr, 0, nullptr }
            #endif

            #ifdef _KMS_WINDOWS_
                { "Interface", PATH_LENGTH + sizeof(uint8_t) * 8, &GUID_TYPE },
            #endif

            { nullptr, 0, nullptr }
        };

        const DI2::Struct<Device2_Config_FIELDS> Device2_Config_TYPE;

    }
}

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

#ifdef _KMS_WINDOWS_

    void GUID_Type::Code_ASCII(const void* aData, DI2::Output* aOutput) const
    {
        assert(nullptr != aData);
        assert(nullptr != aOutput);

        auto lData = reinterpret_cast<const GUID*>(aData);

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

    void GUID_Type::Code_JSON(const void* aData, DI2::Output* aOutput) const
    {
        // TODO JSON
        assert(false);
    }

    // Operator =
    //
    // "{00000000-0000-0000-0000-000000000000}"
    // op "{00000000-0000-0000-0000-000000000000}"
    void GUID_Type::Decode_ASCII(void* aData, KMS::DI2::Input* aInput) const
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

        auto lData = reinterpret_cast<GUID*>(aData);

        *lData = Convert::ToGUID(lStr);
    }

    void GUID_Type::Decode_JSON(void* aData, KMS::DI2::Input* aInput) const
    {
        // TODO JSON
        assert(false);
    }

#endif