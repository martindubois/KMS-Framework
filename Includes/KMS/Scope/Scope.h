
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

        class Scope final
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

            static const char* MODE_NAMES[];

            static const uint16_t FREQUENCY_DEFAULT_Hz;
            static const uint16_t FREQUENCY_MAX_Hz;
            static const uint16_t FREQUENCY_MIN_Hz;
            static const Mode     MODE_DEFAULT;
            static const uint16_t PERSISTENCE_DEFAULT;
            static const uint16_t PERSISTENCE_MIN;
            static const uint16_t POSITION_X_DEFAULT_px;
            static const uint16_t POSITION_X_MAX_px;
            static const uint16_t SCALE_X_DEFAULT_ms_px;
            static const uint16_t SCALE_X_MAX_ms_px;
            static const uint16_t SCALE_X_MIN_ms_px;

            uint16_t mFrequency_Hz;
            Mode     mMode;
            uint16_t mPersistence;
            uint16_t mPositionX_px;
            uint16_t mScaleX_ms_px;

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

            const Callback<Scope> ON_ITERATE;

            Scope();

            ~Scope();

            const char* GetModeName() const;

            void Clear();

            void ForceTrig();

            void Sample();

            void Start();

            void Stop();

            // aChannel  The Channel instance is deleted when the Scope
            //           instance is deleted.
            void Channel_Add(Channel* aChannel);

            void Channel_Remove(Channel* aChannel);

            unsigned int Channels_GetCount() const;

            Graph::Bitmap mBitmap;

            Callback_Ptr mOnFrame;
            Callback_Ptr mOnTrig;

            Stats::Stats mStats_InterSample;

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
