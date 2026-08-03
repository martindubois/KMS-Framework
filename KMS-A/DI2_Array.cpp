
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_BitField.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Array.h>

// ===== Local ==============================================================
#include "DI2_Regex.h"

namespace KMS
{
    namespace DI2
    {

        const std::regex Array_REGEX_GROUP_BEGIN(DI2_Regex_GROUP_BEGIN);
        const std::regex Array_REGEX_GROUP_END  (DI2_Regex_GROUP_END);

        const std::regex Array_REGEX_INDEX_DEC_C1(DI2_Regex_INDEX_DEC_C1);
        const std::regex Array_REGEX_INDEX_HEX_C1(DI2_Regex_INDEX_HEX_C1);

        const std::regex Array_REGEX_VALUE_END(DI2_Regex_BEGIN DI2_Regex_SPACE "," DI2_Regex_SPACE);

    }
}
