
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

        AnalogOutput::AnalogOutput(IAnalogOutputs* aInterface, Id aId) : mId(aId), mInterface(aInterface)
        {
            assert(NULL != aInterface);
        }

        AnalogValue AnalogOutput::Get() const { assert(NULL != mInterface); return mInterface->AO_Get(mId); }

        AnalogValue_Raw AnalogOutput::Get_Raw() const { assert(NULL != mInterface); return mInterface->AO_Get_Raw(mId); }

        void AnalogOutput::Write(AnalogValue aValue) { assert(NULL != mInterface); mInterface->AO_Write(mId, aValue); }

        void AnalogOutput::Write_Raw(AnalogValue_Raw aValue) { assert(NULL != mInterface); mInterface->AO_Write_Raw(mId, aValue); }

    }
}
