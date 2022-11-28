
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/DigitalOutput.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAQ.h>

namespace KMS
{
    namespace DAQ
    {

        class IDigitalOutputs;

        class DigitalOutput
        {

        public:

            DigitalOutput();
            DigitalOutput(IDigitalOutputs* aInterface, Id aId);

            void operator = (const DigitalOutput& aIn);

            void Clear();

            bool Get() const;

            void Set(bool aValue = true);

        private:

            Id               mId;
            IDigitalOutputs* mInterface;

        };

    }
}
