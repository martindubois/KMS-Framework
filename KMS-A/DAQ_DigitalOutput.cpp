
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DAC_DigitalOutput.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DAQ/IDigitalOutputs.h>

#include <KMS/DAQ/DigitalOutput.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        DigitalOutput::DigitalOutput() : mId(0), mInterface(nullptr) {}

        DigitalOutput::DigitalOutput(IDigitalOutputs* aInterface, Id aId) : mId(aId), mInterface(aInterface)
        {
            // assert(nullptr != aInterface);
        }

        void DigitalOutput::operator = (const DigitalOutput& aIn)
        {
            mId        = aIn.mId;
            mInterface = aIn.mInterface;
        }

        void DigitalOutput::Clear() { mInterface->DO_Clear(mId); }

        bool DigitalOutput::Get() const { return mInterface->DO_Get(mId); }

        void DigitalOutput::Set(bool aValue) { mInterface->DO_Set(mId, aValue); }

    }
}
