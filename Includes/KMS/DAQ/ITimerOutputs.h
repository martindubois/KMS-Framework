
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/ITimerOutputs.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAC.h>

namespace KMS
{
    namespace DAQ
    {

        class TimerOutput
        {

        public:

            virtual bool IsRunning(Id aId) const;

            virtual void Start(Id aId, Period aT_us, Period aPW_us = 0, uint32 aCount = 0) = 0;

            virtual void Stop(Id aId) = 0;

        };

    }
}
