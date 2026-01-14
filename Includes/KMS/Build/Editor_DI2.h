
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Editor_DI2.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Struct.h>

namespace KMS
{
    namespace Build
    {

        extern const DI2::Struct_Field Editor_Config_FIELDS[4];

        extern const class DI2::Struct<Editor_Config_FIELDS> Editor_Config_TYPE;

    }
}
