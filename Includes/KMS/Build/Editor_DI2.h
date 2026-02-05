
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2025-2026 KMS
/// \file      Includes/KMS/Build/Editor_DI2.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Struct.h>

namespace KMS
{
    namespace Build
    {

        /// \brief Field list
        /// \see Editor_Config
        extern const DI2::Struct_Field Editor_Config_FIELDS[4];

        /// \brief The `DI2` type description for `Editor_Config`
        /// \see Editor_Config
        extern const class DI2::Struct<Editor_Config_FIELDS> Editor_Config_TYPE;

    }
}
