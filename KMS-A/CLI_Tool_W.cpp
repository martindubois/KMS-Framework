
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_Tool_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/CLI/Tool.h>

namespace KMS
{
    namespace CLI
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Tool::Delay(unsigned int aDelay_ms)
        {
            Sleep(aDelay_ms);
        }

    }
}
