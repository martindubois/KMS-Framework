
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_UInt.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/UInt.h>

// ===== Local ==============================================================
#include "DI2_Regex.h"

namespace KMS
{
    namespace DI2
    {

        const UInt<uint64_t> TYPE_UINT64;
        const UInt<uint32_t> TYPE_UINT32;
        const UInt<uint16_t> TYPE_UINT16;
        const UInt<uint8_t > TYPE_UINT8 ;

        const std::regex UInt_REGEX_OP_VALUE_DEC_C2(DI2_Regex_BEGIN DI2_Regex_OP_C1      DI2_Regex_DEC_C1);
        const std::regex UInt_REGEX_OP_VALUE_HEX_C2(DI2_Regex_BEGIN DI2_Regex_OP_C1      DI2_Regex_HEX_C1);
        const std::regex UInt_REGEX_VALUE_DEC_C1   (DI2_Regex_BEGIN DI2_Regex_ASSIGN_OPT DI2_Regex_DEC_C1);
        const std::regex UInt_REGEX_VALUE_HEX_C1   (DI2_Regex_BEGIN DI2_Regex_ASSIGN_OPT DI2_Regex_HEX_C1);

    }
}
