
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Scope_Scope.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <thread>

// ===== Includes ===========================================================
#include <KMS/OS.h>
#include <KMS/Scope/Channel.h>

#include <KMS/Scope/Scope.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define FADE_COUNT (12)

// Constants
// //////////////////////////////////////////////////////////////////////////

#define PERIOD_DEFAULT_us ( 100000)
#define PERIOD_MAX_us     (1000000)
#define PERIOD_MIN_us     (    100)

namespace KMS
{
    namespace Scope
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // ===== Configurable attributes ====================================
        const uint32_t    Scope::FREQUENCY_DEFAULT_Hz  =      10;
        const uint32_t    Scope::FREQUENCY_MAX_Hz      =   10000;
        const uint32_t    Scope::FREQUENCY_MIN_Hz      =       1;
        const Scope::Mode Scope::MODE_DEFAULT          = Mode::CONTINUOUS_SCROLLING;
        const uint16_t    Scope::POSITION_X_DEFAULT_px =       0;
        const uint16_t    Scope::POSITION_X_MAX_px     =    1024;
        const uint32_t    Scope::SCALE_X_DEFAULT_us_px =  100000;
        const uint32_t    Scope::SCALE_X_MAX_us_px     = 1000000;
        const uint32_t    Scope::SCALE_X_MIN_us_px     =     100;

        Scope::Scope()
            : ON_ITERATE(this, &Scope::OnIterate)
            , mFadeCount(0)
            , mPeriod_us(PERIOD_DEFAULT_us)
            , mX_frac(0)
            , mX_px(0)
            // ===== Configurable attributes ================================
            , mFrequency_Hz(FREQUENCY_DEFAULT_Hz)
            , mMode        (MODE_DEFAULT)
            , mPositionX_px(POSITION_X_DEFAULT_px)
            , mScaleX_us_px(SCALE_X_DEFAULT_us_px)
        {
            State_STOPPED();
        }

        void Scope::Clear()
        {
            auto lUR = mBitmap.GetUpperRight();

            mBitmap.SetBox(Graph::Point::ORIGIN, lUR, Graph::Color::BLACK);
        }

        void Scope::ForceTrig()
        {
            assert(State::QTY > mState);

            if (State::WAITING == mState)
            {
                State_RUNNING();
            }
        }

        void Scope::Sample()
        {
            Fade();

            auto lTrig = false;

            for (auto lC : mChannels)
            {
                assert(nullptr != lC);

                unsigned int lY_px;

                lC->Sample(&lY_px, &lTrig);

                mBitmap.SetPixel(Graph::Point(mX_px, lY_px), lC->mColor);
            }

            switch (mState)
            {
            case State::RUNNING: Run(); break;
            case State::STOPPED: break;
            case State::WAITING: Wait(lTrig); break;

            default: assert(false);
            }
        }

        void Scope::Start() { State_WAITING(); }
        void Scope::Stop () { State_STOPPED(); }

        void Scope::Channel_Add(Channel* aChannel)
        {
            assert(nullptr != aChannel);

            mChannels.push_back(aChannel);
        }

        void Scope::Channel_Remove(Channel* aChannel)
        {
            assert(nullptr != aChannel);

            mChannels.remove(aChannel);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Scope::Fade()
        {
            assert(FADE_COUNT >= mFadeCount);

            if (0 >= mFadeCount)
            {
                auto lUR = mBitmap.GetUpperRight();

                mBitmap.SetBox(Graph::Point::ORIGIN, lUR, 0x010101, Graph::Bitmap::Operation::OP_SUB);

                mFadeCount = FADE_COUNT;
            }
            else
            {
                mFadeCount--;
            }
        }

        void Scope::NextX()
        {
            assert(PERIOD_MAX_us >= mPeriod_us);
            assert(PERIOD_MIN_us <= mPeriod_us);
            assert(SCALE_X_MAX_us_px >= mScaleX_us_px);
            assert(SCALE_X_MIN_us_px <= mScaleX_us_px);

            mX_px++;

            if (mScaleX_us_px > mPeriod_us)
            {
                mX_frac = mScaleX_us_px / mPeriod_us - 1;
            }
            else
            {
                mX_frac = 0;
            }
        }

        void Scope::Run()
        {
            auto lSizeX_px = mBitmap.GetSizeX_px();
            assert(0 < lSizeX_px);

            if (0 < mX_frac)
            {
                mX_frac--;
            }
            else
            {
                NextX();
                if (lSizeX_px <= mX_px)
                {
                    switch (mMode)
                    {
                    case Mode::AUTO:
                    case Mode::CONTINUOUS:
                    case Mode::NORMAL:
                        State_WAITING();
                        mOnFrame.Send(this);
                        break;

                    case Mode::CONTINUOUS_SCROLLING:
                    case Mode::SINGLE_SCROLLING:
                        Scroll(lSizeX_px - 1);
                        mX_px--;
                        break;

                    case Mode::SINGLE:
                        State_STOPPED();
                        mOnFrame.Send(this);
                        break;

                    default: assert(false);
                    }
                }
            }
        }

        void Scope::Scroll(unsigned int aX1_px)
        {
            auto lUR = mBitmap.GetUpperRight();

            if (lUR.mX_px <= aX1_px)
            {
                mBitmap.Scroll_Left(1);
            }
            else if (0 < aX1_px)
            {
                mBitmap.Scroll_Left(Graph::Point::ORIGIN, Graph::Point(aX1_px, lUR.mY_px), 1);
            }
            else
            {
                mBitmap.SetBox(Graph::Point::ORIGIN, Graph::Point(0, lUR.mY_px), Graph::Color::BLACK);
            }
        }

        void Scope::Wait(bool aTrig)
        {
            assert(POSITION_X_MAX_px >= mPositionX_px);

            switch (mMode)
            {
            case Mode::AUTO:
                if (0 < mWaitCount)
                {
                    mWaitCount--;
                }
                else
                {
                    State_RUNNING();
                }

            case Mode::NORMAL:
            case Mode::SINGLE:
            case Mode::SINGLE_SCROLLING:
                if (0 < mX_frac)
                {
                    mX_frac--;
                }
                else
                {
                    if (mPositionX_px > mX_px)
                    {
                        NextX();
                    }
                    else
                    {
                        Scroll(mPositionX_px);
                    }
                }
                break;

            case Mode::CONTINUOUS:
            case Mode::CONTINUOUS_SCROLLING:
                State_RUNNING();
                Run();
                break;

            default: assert(false);
            }

            if (aTrig)
            {
                State_RUNNING();
                mOnTrig.Send(this);
            }
        }

        void Scope::State_RUNNING()
        {
            assert(State::QTY > mState);

            NextX();

            mState = State::RUNNING;
            mX_px  = mPositionX_px;
        }

        void Scope::State_STOPPED()
        {
            assert(State::QTY > mState);

            mState = State::STOPPED;
            mX_px  = 0;
        }

        void Scope::State_WAITING()
        {
            assert(FREQUENCY_MAX_Hz >= mFrequency_Hz);
            assert(FREQUENCY_MIN_Hz <= mFrequency_Hz);
            assert(SCALE_X_MAX_us_px >= mScaleX_us_px);
            assert(SCALE_X_MIN_us_px <= mScaleX_us_px);
            assert(State::QTY > mState);

            NextX();

            mPeriod_us = 1000000 / mFrequency_Hz;
            assert(PERIOD_MAX_us >= mPeriod_us);
            assert(PERIOD_MIN_us <= mPeriod_us);

            mState      = State::WAITING;
            mTime_100ns = OS::GetSystemTime();
            mWaitCount  = mBitmap.GetSizeX_px() * mScaleX_us_px / mPeriod_us;
            mX_px       = 0;
        }

        unsigned int Scope::OnIterate(void*, void*)
        {
            assert(PERIOD_MAX_us >= mPeriod_us);
            assert(PERIOD_MIN_us <= mPeriod_us);

            auto lNow_100ns = OS::GetSystemTime();

            mTime_100ns += mPeriod_us * 10;

            if (mTime_100ns > lNow_100ns)
            {
                auto lDiff_us = (mTime_100ns - lNow_100ns) / 10;

                std::this_thread::sleep_for(std::chrono::microseconds(lDiff_us));
            }

            Sample();

            return 0;
        }

    }
}
