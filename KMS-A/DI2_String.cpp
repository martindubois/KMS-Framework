
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_String.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/String.h>

// ===== Local ==============================================================
#include "DI2_Regex.h"

namespace KMS
{
    namespace DI2
    {

        const std::regex String_REGEX_OP_VALUE_0_C2(DI2_Regex_BEGIN DI2_Regex_OP_C1 DI2_Regex_SPACE "\"(.*)\"" DI2_Regex_SPACE);
        const std::regex String_REGEX_OP_VALUE_1_C2(DI2_Regex_BEGIN DI2_Regex_OP_C1 DI2_Regex_SPACE "(.*)"     DI2_Regex_SPACE);

        const std::regex String_REGEX_VALUE_0_C1(DI2_Regex_BEGIN DI2_Regex_SPACE "\"(.*)\"" DI2_Regex_SPACE);
        const std::regex String_REGEX_VALUE_1_C1(DI2_Regex_BEGIN DI2_Regex_SPACE "(.*)"     DI2_Regex_SPACE);

    }
}
