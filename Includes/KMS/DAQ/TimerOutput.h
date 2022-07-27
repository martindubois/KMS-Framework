
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/TimerOutput.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAC.h>

namespace KMS
{
    namespace DAQ
    {

        class ITimerOutputs;

        class TimerOutput
        {

        public:

            TimerOutput(ITimerOutputs* aInterface, unint8_t aId);

            bool IsRunning() const;

            void Start(Period aT_us, Period aPW_us = 0, Count aCount = 0);

            void Stop();

        private:

            Id        mId;
            ITimerOutputs* mInterface;

        };

    }
}
