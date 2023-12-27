
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_Module.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/Module.h>

namespace KMS
{
    namespace CLI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const int Module::UNKNOWN_COMMAND = 0x80000000;

        void Module::SetConsole(Console::Console* aConsole)
        {
            assert(nullptr != aConsole);

            assert(nullptr == mConsole);

            mConsole = aConsole;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Module::Module() : mConsole(nullptr) {}

    }
}
