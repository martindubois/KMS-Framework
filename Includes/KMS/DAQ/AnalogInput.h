
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/AnalogInput.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAQ.h>

namespace KMS
{
    namespace DAQ
    {

        class IAnalogInputs;

        class AnalogInput
        {

        public:

            AnalogInput();
            AnalogInput(IAnalogInputs* aInterface, Id aId);

            void operator = (const AnalogInput& aIn);

            AnalogValue Read();

            AnalogValue_Raw Read_Raw();

        private:

            Id             mId;
            IAnalogInputs* mInterface;

        };

    }
}
