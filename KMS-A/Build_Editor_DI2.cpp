
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Editor_DI2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Build/Editor.h>
#include <KMS/DI2/String.h>
#include <KMS/String.h>
#include <KMS/String_DI2.h>
#include <KMS/Version_DI2.h>

#include <KMS/Build/Editor_DI2.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI2::String<PATH_LENGTH> VERSION_FILE_TYPE;

namespace KMS
{
    namespace Build
    {

        const DI2::Struct_Field Editor_Config_FIELDS[4] =
        {
            { "Operations" , offsetof(Editor_Config, mOperations ), &StringList_ASCII_TYPE },
            { "Version"    , offsetof(Editor_Config, mVersion    ), &Version_TYPE          },
            { "VersionFile", offsetof(Editor_Config, mVersionFile), &VERSION_FILE_TYPE     },

            { nullptr, 0, nullptr }
        };

        const DI2::Struct<Editor_Config_FIELDS> Editor_Config_TYPE;

    }
}
