
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DAC_DigitalInput.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DAQ/IDigitalInputs.h>

#include <KMS/DAQ/DigitalInput.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        DigitalInput::DigitalInput() : mId(0), mInterface(nullptr) {}

        DigitalInput::DigitalInput(IDigitalInputs* aInterface, Id aId) : mId(aId), mInterface(aInterface)
        {
            // assert(nullptr != aInterface);
        }

        void DigitalInput::operator = (const DigitalInput& aIn)
        {
            mId        = aIn.mId;
            mInterface = aIn.mInterface;
        }

        bool DigitalInput::Read() { return mInterface->DI_Read(mId); }

    }
}
