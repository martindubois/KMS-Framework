
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Thread_Thread_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Thread/Thread.h>

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

extern "C"
{
    static void* Run_Link(void* aArg);
}

namespace KMS
{
    namespace Thread
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Thread::Start()
        {
            KMS_EXCEPTION_ASSERT(State::STOPPED == mState, RESULT_INVALID_STATE, "The thread is not stopped", static_cast<unsigned int>(mState));

            CloseIfNeeded();

            mState = State::STARTING;

            auto lRet = pthread_create(&mThread, nullptr, Run_Link, this);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Thread::Construct_OSDep() {}

        void Thread::CloseIfNeeded() {}

        void Thread::Wait_OSDep(unsigned int aTimeout_ms)
        {
            void* lReturnCode;

            auto lRet = pthread_join(mThread, &lReturnCode);
            assert(0 == lRet);

            assert(nullptr == lReturnCode);
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void* Run_Link(void* aArg)
{
    assert(nullptr != aArg);

    auto lThis = reinterpret_cast<Thread::Thread*>(aArg);

    lThis->Run();

    return nullptr;
}
