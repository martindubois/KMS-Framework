
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Thread_Lock.cpp

// TEST COVERAGE 2022-06-09 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Thread/Gate.h>

#include <KMS/Thread/Lock.h>

namespace KMS
{
    namespace Thread
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Lock::Lock(Gate* aGate) : mGate(aGate), mLocked(true)
        {
            assert(NULL != aGate);

            mGate->Lock();
        }

        Lock::~Lock()
        {
            assert(NULL != mGate);

            if (mLocked)
            {
                mGate->Unlock();
            }
        }

        void Lock::Relock()
        {
            assert(!mLocked);

            mLocked = true;
            mGate->Lock();
        }

        void Lock::Unlock()
        {
            assert(NULL != mGate);
            assert(mLocked);

            mGate->Unlock();
            mLocked = false;
        }

    }
}
