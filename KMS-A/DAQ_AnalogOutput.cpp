
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DAC_AnalogOutput.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DAQ/IAnalogOutputs.h>

#include <KMS/DAQ/AnalogOutput.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        AnalogOutput::AnalogOutput() : mId(0), mInterface(NULL) {}

        AnalogOutput::AnalogOutput(IAnalogOutputs* aInterface, Id aId) : mId(aId), mInterface(aInterface)
        {
            // assert(NULL != aInterface);
        }

        void AnalogOutput::operator = (const AnalogOutput& aIn)
        {
            mId        = aIn.mId;
            mInterface = aIn.mInterface;
        }

        AnalogValue AnalogOutput::Get() const { return mInterface->AO_Get(mId); }

        AnalogValue_Raw AnalogOutput::Get_Raw() const { return mInterface->AO_Get_Raw(mId); }

        void AnalogOutput::Write(AnalogValue aValue) { mInterface->AO_Write(mId, aValue); }

        void AnalogOutput::Write_Raw(AnalogValue_Raw aValue) { mInterface->AO_Write_Raw(mId, aValue); }

    }
}
