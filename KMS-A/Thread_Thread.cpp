
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Thread_Thread.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Thread/Lock.h>

#include <KMS/Thread/Thread.h>

namespace KMS
{
    namespace Thread
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Thread::Thread() : mState(State::STOPPED)
        {
            Construct_OSDep();
        }

        Thread::~Thread()
        {
            Lock lLock(&mGate);

            switch (mState)
            {
            case State::RUNNING:
            case State::STARTING:
                mState = State::STOPPING;
                // no break

            case State::STOPPING:
                lLock.Unlock();
                Wait(2000);
                break;

            case State::STOPPED: break;

            default: assert(false);
            }

            CloseIfNeeded();
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
            assert(CALLBACK_SUCCESS_OR_WARNING(lRet));
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
                lLock.Unlock();

                Wait_OSDep(aTimeout_ms);
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
            if (CALLBACK_SUCCESS_OR_WARNING(lRet))
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
                            assert(CALLBACK_SUCCESS(lRet));
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

                            if (!CALLBACK_SUCCESS(lRet))
                            {
                                break;
                            }

                            if (0 != (lRet & ICallback::FLAG_ACTION_STOP))
                            {
                                break;
                            }
                        }
                    }

                    lLock.Unlock();

                    lRet = mOnStopping.Send(this);
                    assert(CALLBACK_SUCCESS_OR_WARNING(lRet));
                }
            }

            mState = State::STOPPED;
        }

    }
}
