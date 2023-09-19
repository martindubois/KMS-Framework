
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Scope/Scope.h

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Graph/Bitmap.h>

namespace KMS
{
    namespace Scope
    {

        class Channel;

        class Scope
        {

        public:

            enum class Mode
            {
                AUTO = 0,
                CONTINUOUS,
                CONTINUOUS_SCROLLING,
                NORMAL,
                SINGLE,
                SINGLE_SCROLLING,

                QTY
            };

            // STOPPED <==+---------+
            //  |   |     |         |
            //  |   +--> WAITING    |
            //  |         |         |
            //  +---------+==> RUNNING
            enum class State
            {
                RUNNING,
                STOPPED,
                WAITING,

                QTY
            };

            static const char* MODE_NAMES[];

            // ===== Configurable attributes ================================
            static const uint32_t FREQUENCY_DEFAULT_Hz;
            static const uint32_t FREQUENCY_MAX_Hz;
            static const uint32_t FREQUENCY_MIN_Hz;
            static const Mode     MODE_DEFAULT;
            static const uint16_t POSITION_X_DEFAULT_px;
            static const uint16_t POSITION_X_MAX_px;
            static const uint32_t SCALE_X_DEFAULT_us_px;
            static const uint32_t SCALE_X_MAX_us_px;
            static const uint32_t SCALE_X_MIN_us_px;

            const Callback<Scope> ON_ITERATE;

            Scope();

            void Clear();

            void ForceTrig();

            void Sample();

            void Start();

            void Stop();

            void Channel_Add(Channel* aChannel);

            void Channel_Remove(Channel* aChannel);

            Graph::Bitmap mBitmap;

            Callback_Ptr mOnFrame;
            Callback_Ptr mOnTrig;

            // ===== Configurable attributes ================================
            uint32_t mFrequency_Hz;
            Mode     mMode;
            uint32_t mPositionX_px;
            uint32_t mScaleX_us_px;

        private:

            void Fade();
            void NextX();
            void Run();
            void Scroll(unsigned int aX1_px);
            void Wait(bool aTrig);

            void State_RUNNING();
            void State_STOPPED();
            void State_WAITING();

            unsigned int OnIterate(void* aSender, void* aData);

            typedef std::list<Channel*> ChannelList;

            ChannelList mChannels;

            unsigned int mFadeCount;
            unsigned int mPeriod_us;

            State mState;

            uint64_t mTime_100ns;

            unsigned int mX_frac;
            unsigned int mX_px;

            unsigned int mWaitCount;

        };

    }
}
