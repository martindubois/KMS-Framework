
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Float.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Float.h>

// ===== Local ==============================================================
#include "DI2_Regex.h"

namespace KMS
{
    namespace DI2
    {

        const Float<double> TYPE_DOUBLE;
        const Float<float > TYPE_FLOAT;

        const std::regex Float_REGEX_OP_VALUE_C2(DI2_Regex_BEGIN DI2_Regex_OP_C1      DI2_Regex_SPACE "(\\-?\\d+\\.?\\d*)" DI2_Regex_SPACE);
        const std::regex Float_REGEX_VALUE_C1   (DI2_Regex_BEGIN DI2_Regex_ASSIGN_OPT DI2_Regex_SPACE "(\\-?\\d+\\.?\\d*)" DI2_Regex_SPACE);

    }
}
