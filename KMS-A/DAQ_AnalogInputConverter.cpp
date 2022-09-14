
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DAC_AnalogInputConverter.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DAQ/AnalogInputConverter.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        AnalogInputConverter::AnalogInputConverter(IAnalogInputs* aInterface) : mInterface(aInterface)
        {
            assert(NULL != aInterface);
        }

        // ===== IAnalogInputes =============================================
        AnalogValue AnalogInputConverter::AI_Read(Id aId)
        {
            assert(NULL != mInterface);

            return Scale(aId, mInterface->AI_Read(aId));
        }

        AnalogValue_Raw AnalogInputConverter::AI_Read_Raw(Id aId)
        {
            assert(NULL != mInterface);

            return mInterface->AI_Read_Raw(aId);
        }

    }
}
