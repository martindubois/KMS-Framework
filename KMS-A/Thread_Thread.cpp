
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Thread_Thread.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>
#include <KMS/Thread/Lock.h>

#include <KMS/Thread/Thread.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static DWORD WINAPI Run_Link(LPVOID aParam);

namespace KMS
{
    namespace Thread
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Thread::Thread()
            : mHandle(NULL)
            , mState(State::STOPPED)
        {
        }

        Thread::~Thread()
        {
            Lock lLock(&mGate);

            switch (mState)
            {
            case State::RUNNING:
            case State::STARTING: mState = State::STOPPING; // no break

            case State::STOPPING: Wait(2000); break;

            case State::STOPPED: break;

            default: assert(false);
            }

            lLock.Unlock();

            CloseIfNeeded();
        }

        void Thread::Start()
        {
            KMS_EXCEPTION_ASSERT(State::STOPPED == mState, THREAD_STATE_INVALID, "The thread is not stopped", static_cast<unsigned int>(mState));

            CloseIfNeeded();

            assert(NULL == mHandle);

            mState = State::STARTING;

            mHandle = CreateThread(NULL, 0, Run_Link, this, 0, NULL);
            if (NULL == mHandle)
            {
                mState = State::STOPPED;
                KMS_EXCEPTION(THREAD_START_FAILED, "Cannot create the thread", "");
            }
        }

        void Thread::Stop()
        {
            Lock lLock(&mGate);

            switch (mState)
            {
            case State::RUNNING:
            case State::STARTING: mState = State::STOPPING; break;

            case State::STOPPED:
            case State::STOPPING:
                break;

            default: assert(false);
            }

            lLock.Unlock();

            unsigned int lRet = mOnStop.Send(this);
            assert(KMS_MSG_SUCCESS_OR_WARNING(lRet));
        }

        void Thread::StopAndWait(unsigned int aTimeout_ms)
        {
            Stop();
            Wait(aTimeout_ms);
        }

        void Thread::Wait(unsigned int aTimeout_ms)
        {
            Lock lLock(&mGate);

            switch (mState)
            {
            case State::RUNNING:
            case State::STARTING:
            case State::STOPPING:
                assert(NULL != mHandle);

                lLock.Unlock();

                DWORD lRet;

                lRet = WaitForSingleObject(mHandle, aTimeout_ms);
                assert(WAIT_OBJECT_0 == lRet);
                break;

            case State::STOPPED: break;

            default: assert(false);
            }
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Thread::Run()
        {
            unsigned int lRet = mOnStarting.Send(this);
            if (KMS_MSG_SUCCESS_OR_WARNING(lRet))
            {
                Lock lLock(&mGate);

                if (State::STARTING == mState)
                {
                    mState = State::RUNNING;

                    if (mOnRun.IsSet())
                    {
                        lLock.Unlock();
                        {
                            lRet = mOnRun.Send(this);
                            assert(KMS_MSG_SUCCESS(lRet));
                        }
                        lLock.Relock();

                        mState = State::STOPPING;
                    }
                    else
                    {
                        while (State::RUNNING == mState)
                        {
                            lLock.Unlock();
                            {
                                lRet = mOnIterate.Send(this);
                            }
                            lLock.Relock();

                            if (!KMS_MSG_SUCCESS(lRet))
                            {
                                break;
                            }

                            if (0 != (lRet & Msg::IReceiver::MSG_ACTION_STOP))
                            {
                                break;
                            }
                        }
                    }

                    lLock.Unlock();

                    lRet = mOnStopping.Send(this);
                    assert(KMS_MSG_SUCCESS_OR_WARNING(lRet));
                }
            }

            mState = State::STOPPED;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Thread::CloseIfNeeded()
        {
            if (NULL != mHandle)
            {
                bool lRet = CloseHandle(mHandle);
                assert(lRet);

                mHandle = NULL;
            }
        }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

DWORD WINAPI Run_Link(LPVOID aParam)
{
    assert(NULL != aParam);

    KMS::Thread::Thread* lThis = reinterpret_cast<KMS::Thread::Thread*>(aParam);

    lThis->Run();

    return 0;
}
