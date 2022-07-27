
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

        DigitalInput::DigitalInput(IDigitalInputs* aInterface, Id aId) : mId(aId), mInterface(aInterface)
        {
            assert(NULL != aInterface);
        }

        bool DigitalInput::Read() { assert(NULL != mInterface); return mInterface->DI_Read(mId); }

    }
}
