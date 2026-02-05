
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2025-2026 KMS
/// \file      Includes/KMS/Build/Editor.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Base.h>
#include <KMS/Configurable.h>
#include <KMS/Types.h>
#include <KMS/Version.h>

namespace KMS
{
    namespace Build
    {

        /// \brief Configuration for the Editor class
        /// \note This is what, here at KMS, we call a configuration class.
        ///       - copy allowed
        ///       - final
        ///       - no virtual method
        ///       - public members
        ///       - uses `KMS_Config_METHODS` macro to declare standard
        ///         configurtion methodes
        class Editor_Config final
        {

        public:

            KMS_Config_METHODS(Editor_Config);

            StringList_ASCII mOperations;
            Version          mVersion;
            char             mVersionFile[PATH_LENGTH];

        };

        /// \brief Editor used to replace KMS software version in source
        ///        files
        class Editor final : public Configurable<Editor_Config>
        {

        public:

            void Execute();

        };

    }
}
