
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_BitField.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/BitField.h>

// ===== Local ==============================================================
#include "DI2_Regex.h"

namespace KMS
{
    namespace DI2
    {

        const std::regex BitField_REGEX_DEC_C3(DI2_Regex_BEGIN DI2_Regex_SPACE DI2_Regex_NAME_C1 DI2_Regex_OP_C1 DI2_Regex_DEC_C1);
        const std::regex BitField_REGEX_HEX_C3(DI2_Regex_BEGIN DI2_Regex_SPACE DI2_Regex_NAME_C1 DI2_Regex_OP_C1 DI2_Regex_HEX_C1);

        const std::regex BitField_REGEX_FIELD_END(DI2_Regex_FIELD_END);

        const std::regex BitField_REGEX_FIELD_OP_VALUE_DEC_0_C3(DI2_Regex_BEGIN DI2_Regex_SPACE DI2_Regex_NAME_C1 DI2_Regex_OP_C1 DI2_Regex_DEC_C1);
        const std::regex BitField_REGEX_FIELD_OP_VALUE_DEC_1_C3(DI2_Regex_BEGIN DI2_Regex_DOT   DI2_Regex_NAME_C1 DI2_Regex_OP_C1 DI2_Regex_DEC_C1);
        const std::regex BitField_REGEX_FIELD_OP_VALUE_HEX_0_C3(DI2_Regex_BEGIN DI2_Regex_SPACE DI2_Regex_NAME_C1 DI2_Regex_OP_C1 DI2_Regex_HEX_C1);
        const std::regex BitField_REGEX_FIELD_OP_VALUE_HEX_1_C3(DI2_Regex_BEGIN DI2_Regex_DOT   DI2_Regex_NAME_C1 DI2_Regex_OP_C1 DI2_Regex_HEX_C1);

        const std::regex BitField_REGEX_GROUP_BEGIN(DI2_Regex_GROUP_BEGIN);
        const std::regex BitField_REGEX_GROUP_END  (DI2_Regex_GROUP_END);

        const std::regex BitField_REGEX_OP_VALUE_DEC_C2(DI2_Regex_BEGIN DI2_Regex_OP_C1 DI2_Regex_DEC_C1);
        const std::regex BitField_REGEX_OP_VALUE_HEX_C2(DI2_Regex_BEGIN DI2_Regex_OP_C1 DI2_Regex_HEX_C1);

        const std::regex BitField_REGEX_VALUE_DEC_C1(DI2_Regex_BEGIN DI2_Regex_DEC_C1);
        const std::regex BitField_REGEX_VALUE_HEX_C1(DI2_Regex_BEGIN DI2_Regex_HEX_C1);

    }
}
