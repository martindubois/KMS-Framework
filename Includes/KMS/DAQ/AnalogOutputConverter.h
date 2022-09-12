
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/AnalogOutputConverter.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/AnalogOutput.h>

namespace KMS
{
    namespace DAQ
    {

        class AnalogOutputConverter
        {

        public:

            AnalogOutputConverter(const AnalogOutput& aOutput, double aScale, double aOffset = 0.0);

            AnalogValue Get() const;

            void Write(AnalogValue aValue);

        private:

            AnalogOutput mOutput;

            double mOffset;
            double mScale;

        };

    }
}
