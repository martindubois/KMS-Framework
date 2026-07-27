
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device2_DI2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dev/Device2.h>
#include <KMS/DI2/String.h>
#include <KMS/DI2/UInt.h>

#ifdef _KMS_WINDOWS_
    #include <KMS/DI2/GUID.h>
#endif

#include <KMS/Dev/Device2_DI2.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI2::String<PATH_LENGTH> PATH_TYPE;

namespace KMS
{
    namespace Dev
    {

        const DI2::Struct_Field Device2_Config_FIELDS[5] =
        {
            { "Index"  , offsetof(Device2_Config, mIndex  ), &DI2::TYPE_UINT8 },
            { "Link"   , offsetof(Device2_Config, mLink   ), &PATH_TYPE       },
            { "Pattern", offsetof(Device2_Config, mPattern), &PATH_TYPE       },

            #ifdef _KMS_LINUX_
                { nullptr, 0, nullptr }
            #endif

            #ifdef _KMS_WINDOWS_
                { "Interface", offsetof(Device2_Config, mInterface), &DI2::TYPE_GUID },
            #endif

            { nullptr, 0, nullptr }
        };

        const DI2::Struct<Device2_Config_FIELDS> Device2_Config_TYPE;

    }
}
