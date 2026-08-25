
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Enum.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Enum.h>

// ===== Local ==============================================================
#include "DI2_Regex.h"

namespace KMS
{
    namespace DI2
    {

        const std::regex Enum_REGEX_VALUE_NAME_C1(DI2_Regex_BEGIN DI2_Regex_ASSIGN_OPT DI2_Regex_SPACE DI2_Regex_NAME_C1 DI2_Regex_SPACE);

    }
}
