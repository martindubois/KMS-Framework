
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Thread_Thread_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Thread/Thread.h>

KMS_RESULT_STATIC(RESULT_START_FAILED);

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static DWORD WINAPI Run_Link(LPVOID aParam);

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

            assert(nullptr == mHandle);

            mState = State::STARTING;

            mHandle = CreateThread(NULL, 0, Run_Link, this, 0, NULL);
            if (nullptr == mHandle)
            {
                mState = State::STOPPED;
                KMS_EXCEPTION(RESULT_START_FAILED, "Cannot create the thread", "");
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Thread::Construct_OSDep()
        {
            mHandle = nullptr;
        }

        void Thread::CloseIfNeeded()
        {
            if (nullptr != mHandle)
            {
                auto lRet = CloseHandle(mHandle);
                assert(lRet);

                mHandle = nullptr;
            }
        }

        void Thread::Wait_OSDep(unsigned int aTimeout_ms)
        {
            assert(nullptr != mHandle);

            auto lRet = WaitForSingleObject(mHandle, aTimeout_ms);
            assert(WAIT_OBJECT_0 == lRet);
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

DWORD WINAPI Run_Link(LPVOID aParam)
{
    assert(nullptr != aParam);

    auto lThis = reinterpret_cast<Thread::Thread*>(aParam);

    lThis->Run();

    return 0;
}
