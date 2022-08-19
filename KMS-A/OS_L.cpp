
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/OS_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== Includes ===========================================================
#include <KMS/OS.h>

namespace KMS
{
    namespace OS
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        uint32_t GetProcessId() { return getpid(); }

        uint64_t GetSystemTime()
        {
            struct timespec lTime;

            clock_gettime(CLOCK_REALTIME, &lTime);

            uint64_t lResult_100ns = lTime.tv_sec;

            lResult_100ns *= 10000000;
            lResult_100ns += lTime.tv_nsec / 100;

            return lResult_100ns;
        }

        uint32_t GetThreadId() { return gettid(); }

    }
}
