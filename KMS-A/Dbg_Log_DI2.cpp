
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log_DI2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Enum.h>
#include <KMS/DI2/String.h>
#include <KMS/Dbg/Log.h>

#include <KMS/Dbg/Log_DI2.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI2::Enum<KMS::Dbg::ConsoleMode, KMS::Dbg::ConsoleMode_NAMES> CONSOLE_MODE_TYPE;
static const KMS::DI2::Enum<KMS::Dbg::Level, KMS::Dbg::Level_NAMES> LEVEL_TYPE;
static const KMS::DI2::String<PATH_LENGTH> PATH_TYPE;

namespace KMS
{
    namespace Dbg
    {

        const DI2::Struct_Field Log_Config_FIELDS[5] =
        {
            { "ConsoleLevel", offsetof(Log_Config, mConsoleLevel), &LEVEL_TYPE },
            { "ConsoleMode" , offsetof(Log_Config, mConsoleMode ), &CONSOLE_MODE_TYPE },
            { "FileLevel"   , offsetof(Log_Config, mFileLevel   ), &LEVEL_TYPE },
            { "Folder"      , offsetof(Log_Config, mFolder      ), &PATH_TYPE },

            { nullptr, 0, nullptr }
        };

        const DI2::Struct<Log_Config_FIELDS> Log_Config_TYPE;

    }
}
