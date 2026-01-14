
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Version_DI2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/String.h>
#include <KMS/DI2/UInt.h>
#include <KMS/Version_DI2.h>

namespace KMS
{

    static const DI2::String<16> STRING_16;
    static const DI2::String<27> STRING_27;

    const DI2::Struct_Field Version_FIELDS[8] =
    {
        { "Major" , 0, &DI2::TYPE_UINT8 },
        { "Minor" , 1, &DI2::TYPE_UINT8 },
        { "Build" , 2, &DI2::TYPE_UINT8 },
        { "Compat", 3, &DI2::TYPE_UINT8 },
        { "Debug" , 4, &DI2::TYPE_UINT8 },
        { "Date"  , 5, &STRING_27       },
        { "Type"  , 5, &STRING_16       },

        { nullptr, 0, nullptr }
    };

    const class DI2::Struct<Version_FIELDS> Version_TYPE;

}
