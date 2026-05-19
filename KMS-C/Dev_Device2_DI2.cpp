
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device2_DI2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/String.h>
#include <KMS/DI2/UInt.h>

#ifdef _KMS_WINDOWS_
    #include <KMS/DI2/GUID.h>
#endif

#include <KMS/Dev/Device2_DI2.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI2::String<PATH_LENGTH> LINK_TYPE;

namespace KMS
{
    namespace Dev
    {

        const DI2::Struct_Field Device2_Config_FIELDS[4] =
        {
            { "Link" ,           0, &LINK_TYPE  },
            { "Index", PATH_LENGTH, &DI2::TYPE_UINT8 },

            #ifdef _KMS_LINUX_
                { nullptr, 0, nullptr }
            #endif

            #ifdef _KMS_WINDOWS_
                { "Interface", PATH_LENGTH + sizeof(uint8_t) * 8, &DI2::TYPE_GUID },
            #endif

            { nullptr, 0, nullptr }
        };

        const DI2::Struct<Device2_Config_FIELDS> Device2_Config_TYPE;

    }
}
