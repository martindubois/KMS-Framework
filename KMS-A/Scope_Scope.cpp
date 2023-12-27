
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

// Constants
// //////////////////////////////////////////////////////////////////////////

#define PERIOD_DEFAULT_us ( 100000)
#define PERIOD_MAX_us     (1000000)
#define PERIOD_MIN_us     (   1000)

namespace KMS
{
    namespace Scope
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Scope::MODE_NAMES[] = { "AUTO", "CONTINUOUS", "CONTINUOUS_SCROLLING", "NORMAL", "SINGLE", "SINGLE_SCROLLING" };

        const uint16_t    Scope::FREQUENCY_DEFAULT_Hz  =    10;
        const uint16_t    Scope::FREQUENCY_MAX_Hz      = 10000;
        const uint16_t    Scope::FREQUENCY_MIN_Hz      =     1;
        const Scope::Mode Scope::MODE_DEFAULT          = Mode::CONTINUOUS_SCROLLING;
        const uint16_t    Scope::PERSISTENCE_DEFAULT   =     6;
        const uint16_t    Scope::PERSISTENCE_MIN       =     1;
        const uint16_t    Scope::POSITION_X_DEFAULT_px =     0;
        const uint16_t    Scope::POSITION_X_MAX_px     =  2048;
        const uint16_t    Scope::SCALE_X_DEFAULT_ms_px =   100;
        const uint16_t    Scope::SCALE_X_MAX_ms_px     =  1000;
        const uint16_t    Scope::SCALE_X_MIN_ms_px     =     1;

        Scope::Scope()
            : mFrequency_Hz(FREQUENCY_DEFAULT_Hz)
            , mMode        (MODE_DEFAULT)
            , mPersistence (PERSISTENCE_DEFAULT)
            , mPositionX_px(POSITION_X_DEFAULT_px)
            , mScaleX_ms_px(SCALE_X_DEFAULT_ms_px)
            , ON_ITERATE(this, &Scope::OnIterate)
            , mFadeCount(0)
            , mPeriod_us(PERIOD_DEFAULT_us)
            , mX_frac(0)
            , mX_px(POSITION_X_DEFAULT_px)
        {
            State_STOPPED();
        }

        Scope::~Scope()
        {
            for (auto lChannel : mChannels)
            {
                assert(nullptr != lChannel);

                delete lChannel;
            }
        }

        const char* Scope::GetModeName() const { return Enum<Mode, MODE_NAMES>(mMode).GetName(); }

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

        void Scope::Start() { State_WAITING(); mStats_InterSample.Reset(); }
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

        unsigned int Scope::Channels_GetCount() const
        {
            return static_cast<unsigned int>(mChannels.size());
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Scope::Fade()
        {
            if (0 >= mFadeCount)
            {
                auto lUR = mBitmap.GetUpperRight();

                mBitmap.SetBox(Graph::Point::ORIGIN, lUR, 0x010101, Graph::Bitmap::Operation::OP_SUB);

                mFadeCount = mPersistence;
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
            assert(SCALE_X_MAX_ms_px >= mScaleX_ms_px);
            assert(SCALE_X_MIN_ms_px <= mScaleX_ms_px);

            unsigned int lScaleX_us_px = mScaleX_ms_px * 1000;

            mX_px++;

            if (lScaleX_us_px >= mPeriod_us)
            {
                mX_frac = lScaleX_us_px / mPeriod_us - 1;
            }
            else
            {
                mScaleX_ms_px = mPeriod_us / 1000;
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

            mX_px--;
        }

        void Scope::Wait(bool aTrig)
        {
            assert(POSITION_X_MAX_px >= mPositionX_px);
            assert(mPositionX_px == mX_px);

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
                    NextX();
                    Scroll(mPositionX_px);
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

            mState = State::RUNNING;
        }

        void Scope::State_STOPPED()
        {
            mState = State::STOPPED;
            mX_px  = 0;
        }

        void Scope::State_WAITING()
        {
            assert(FREQUENCY_MAX_Hz >= mFrequency_Hz);
            assert(FREQUENCY_MIN_Hz <= mFrequency_Hz);
            assert(SCALE_X_MAX_ms_px >= mScaleX_ms_px);
            assert(SCALE_X_MIN_ms_px <= mScaleX_ms_px);
            assert(State::QTY > mState);

            NextX();

            mPeriod_us = 1000000 / mFrequency_Hz;
            assert(PERIOD_MAX_us >= mPeriod_us);
            assert(PERIOD_MIN_us <= mPeriod_us);

            mState      = State::WAITING;
            mTime_100ns = OS::GetSystemTime();
            mWaitCount  = mBitmap.GetSizeX_px() * mScaleX_ms_px / mPeriod_us;
            mX_px       = mPositionX_px;
        }

        unsigned int Scope::OnIterate(void*, void*)
        {
            assert(PERIOD_MAX_us >= mPeriod_us);
            assert(PERIOD_MIN_us <= mPeriod_us);

            auto lNow_100ns = OS::GetSystemTime();

            mTime_100ns += mPeriod_us * 10;

            double lIS_us;

            if (mTime_100ns > lNow_100ns)
            {
                auto lDiff_us = (mTime_100ns - lNow_100ns) / 10;

                std::this_thread::sleep_for(std::chrono::microseconds(lDiff_us));

                lIS_us = static_cast<double>(lDiff_us);
            }
            else
            {
                auto lDiff_us = (lNow_100ns - mTime_100ns) / 10;

                lIS_us = -1.0 * lDiff_us;
            }

            mStats_InterSample.AddData(lIS_us / 1000);

            Sample();

            return 0;
        }

    }
}
