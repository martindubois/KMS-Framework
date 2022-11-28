
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DAC_AnalogInput.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DAQ/IAnalogInputs.h>

#include <KMS/DAQ/AnalogInput.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        AnalogInput::AnalogInput() : mId(0), mInterface(NULL) {}

        AnalogInput::AnalogInput(IAnalogInputs* aInterface, Id aId) : mId(aId), mInterface(aInterface)
        {
            // assert(NULL != aInterface);
        }

        void AnalogInput::operator = (const AnalogInput& aIn)
        {
            mId        = aIn.mId;
            mInterface = aIn.mInterface;
        }

        AnalogValue AnalogInput::Read() { return mInterface->AI_Read(mId); }

        AnalogValue_Raw AnalogInput::Read_Raw() { return mInterface->AI_Read_Raw(mId); }

    }
}
