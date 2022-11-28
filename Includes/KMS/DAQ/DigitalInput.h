
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/DigitalInput.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAQ.h>

namespace KMS
{
    namespace DAQ
    {

        class IDigitalInputs;

        class DigitalInput
        {

        public:

            DigitalInput();
            DigitalInput(IDigitalInputs* aInterface, Id aId);

            void operator = (const DigitalInput& aIn);

            bool Read();

        private:

            Id              mId;
            IDigitalInputs* mInterface;

        };

    }
}
