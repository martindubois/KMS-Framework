
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/OS.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/OS.h>

namespace KMS
{
    namespace OS
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        uint32_t GetProcessId() { return GetCurrentProcessId(); }

        uint64_t GetSystemTime()
        {
            uint64_t lResult;

            GetSystemTimeAsFileTime(reinterpret_cast<FILETIME*>(&lResult));

            return lResult;
        }

        uint32_t GetThreadId() { return GetCurrentThreadId(); }

    }
}
