
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Editor.h

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

        class Editor_Config
        {

        public:

            KMS_Config_METHODS(Editor_Config);

            StringList_ASCII mOperations;
            Version          mVersion;
            char             mVersionFile[PATH_LENGTH];

        };

        class Editor : public Configurable<Editor_Config>
        {

        public:

            void Execute();

        };

    }
}
