
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Struct.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Struct.h>

// ===== Local ==============================================================
#include "DI2_Regex.h"

namespace KMS
{
    namespace DI2
    {

        const std::regex Struct_REGEX_FIELD_0_C1(DI2_Regex_BEGIN DI2_Regex_SPACE DI2_Regex_NAME_C1 DI2_Regex_SPACE);
        const std::regex Struct_REGEX_FIELD_1_C1(DI2_Regex_BEGIN DI2_Regex_DOT   DI2_Regex_NAME_C1 DI2_Regex_SPACE);

        const std::regex Struct_REGEX_FIELD_END(DI2_Regex_FIELD_END);

        const std::regex Struct_REGEX_GROUP_BEGIN(DI2_Regex_GROUP_BEGIN);
        const std::regex Struct_REGEX_GROUP_END  (DI2_Regex_GROUP_END);

    }
}
