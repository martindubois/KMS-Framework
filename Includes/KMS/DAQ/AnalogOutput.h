
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/AnalogOutput.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAQ.h>

namespace KMS
{
    namespace DAQ
    {

        class IAnalogOutputs;

        class AnalogOutput final
        {

        public:

            AnalogOutput();
            AnalogOutput(IAnalogOutputs* aInterface, Id aId);

            void operator = (const AnalogOutput& aIn);

            AnalogValue Get() const;

            AnalogValue_Raw Get_Raw() const;

            void Write(AnalogValue aValue);

            void Write_Raw(AnalogValue_Raw aValue);

        private:

            Id              mId;
            IAnalogOutputs* mInterface;

        };

    }
}
