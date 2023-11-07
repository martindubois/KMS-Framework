
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Installer_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Install/Installer.h>

namespace KMS
{
    namespace Install
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Installer::AddToPath()
        {
            KMS_EXCEPTION(RESULT_NOT_IMPLEMENTED, "NotImplemented", "");
        }

        void Installer::RemoveFromPath()
        {
            KMS_EXCEPTION(RESULT_NOT_IMPLEMENTED, "NotImplemented", "");
        }

    }
}
