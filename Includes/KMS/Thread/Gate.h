
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Thread/Gate.h

#pragma once

// ===== Windows ============================================================
#include <Windows.h>

namespace KMS
{
    namespace Thread
    {

        class Gate
        {

        public:

            Gate();

            ~Gate();

            void Lock();
            void Unlock();

        private:

            CRITICAL_SECTION mInternal;

        };

    }
}
