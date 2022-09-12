
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/AnalogInputConverter.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/AnalogInput.h>

namespace KMS
{
    namespace DAQ
    {

        class AnalogInputConverter
        {

        public:

            AnalogInputConverter(const AnalogInput& aInput, double aScale, double aOffset = 0.0);

            AnalogValue Read();

        private:

            AnalogInput mInput;

            double mOffset;
            double mScale;

        };

    }
}
