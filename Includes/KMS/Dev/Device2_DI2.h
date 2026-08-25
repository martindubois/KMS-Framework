
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-C

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2026 KMS
/// \file      Includes/KMS/Dev/Device2_DI2.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Struct.h>

namespace KMS
{
    namespace Dev
    {

        /// \brief Field list
        /// \see Device2_Config
        extern const DI2::Struct_Field Device2_Config_FIELDS[5];

        /// \brief The `DI2` type description for `Device2_Config`
        /// \see Device2_Config
        extern const DI2::Struct<Device2_Config_FIELDS> Device2_Config_TYPE;

    }
}
