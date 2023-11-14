
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Thread_Gate_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Thread/Gate.h>

namespace KMS
{
    namespace Thread
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Gate::Gate() { pthread_mutex_init(&mInternal, nullptr); }

        Gate::~Gate() { pthread_mutex_destroy(&mInternal); }

        void Gate::Lock() { pthread_mutex_lock(&mInternal); }

        void Gate::Unlock() { pthread_mutex_unlock(&mInternal); }

    }
}
