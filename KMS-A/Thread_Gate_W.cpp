
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Thread_Gate_W.cpp

// TEST COVERAGE 2022-06-09 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Thread/Gate.h>

namespace KMS
{
    namespace Thread
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Gate::Gate() { InitializeCriticalSection(&mInternal); }

        Gate::~Gate() { DeleteCriticalSection(&mInternal); }

        void Gate::Lock() { EnterCriticalSection(&mInternal); }

        void Gate::Unlock() { LeaveCriticalSection(&mInternal); }

    }
}
