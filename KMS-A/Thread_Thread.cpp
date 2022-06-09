
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Thread_Thread.cpp

// TODO Use the classes Thread::Gate and Thread::Lock

#include "Component.h"

// ===== Includes ===========================================================
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
            , mOnIterate (this)
            , mOnRun     (this)
            , mOnStarting(this)
            , mOnStop    (this)
            , mOnStopping(this)
            , mState(State::STOPPED)
        {
            InitializeCriticalSection(&mGate);
        }

        Thread::~Thread() { CloseIfNeeded(); }

        void Thread::Lock() { EnterCriticalSection(&mGate); }

        void Thread::Unlock() { LeaveCriticalSection(&mGate); }

        void Thread::Start()
        {
            if (State::STOPPED != mState)
            {
                KMS_EXCEPTION_WITH_INFO(STATE, "The thread is not stopped", static_cast<unsigned int>(mState));
            }

            CloseIfNeeded();

            assert(NULL == mHandle);

            mState = State::STARTING;

            mHandle = CreateThread(NULL, 0, Run_Link, this, 0, NULL);
            if (NULL == mHandle)
            {
                mState = State::STOPPED;
                KMS_EXCEPTION(THREAD_START, "CreateThread failed");
            }
        }

        void Thread::Stop()
        {
            Lock();
            {
                switch (mState)
                {
                case State::RUNNING:
                case State::STARTING: mState = State::STOPPING; break;

                case State::STOPPED:
                case State::STOPPING:
                    break;

                default: assert(false);
                }
            }
            Unlock();

            mOnStop.Send();
        }

        void Thread::StopAndWait(unsigned int aTimeout_ms)
        {
            Stop();
            Wait(aTimeout_ms);
        }

        void Thread::Wait(unsigned int aTimeout_ms)
        {
            Lock();
            {
                switch (mState)
                {
                case State::RUNNING:
                case State::STARTING:
                case State::STOPPING:
                    assert(NULL != mHandle);

                    Unlock();
                    {
                        DWORD lRet = WaitForSingleObject(mHandle, aTimeout_ms);
                        assert(WAIT_OBJECT_0 == lRet);
                    }
                    Lock();
                    break;

                case State::STOPPED: break;

                default: assert(false);
                }
            }
            Unlock();
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Thread::Run()
        {
            if (mOnStarting.Send())
            {
                Lock();
                {
                    if (State::STARTING == mState)
                    {
                        mState = State::RUNNING;

                        if (mOnRun.IsSet())
                        {
                            Unlock();
                            {
                                mOnRun.Send();
                            }
                            Lock();

                            mState = State::STOPPING;
                        }
                        else
                        {
                            while (State::RUNNING == mState)
                            {
                                bool lRet = false;

                                Unlock();
                                {
                                    lRet = mOnIterate.Send();
                                }
                                Lock();

                                if (!lRet)
                                {
                                    break;
                                }
                            }
                        }
                    }

                    Unlock();
                    {
                        mOnStopping.Send();
                    }
                    Lock();
                }
                Unlock();
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
