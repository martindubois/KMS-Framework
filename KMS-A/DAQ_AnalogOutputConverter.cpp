
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DAC_AnalogOutputConverter.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DAQ/AnalogOutputConverter.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        AnalogOutputConverter::AnalogOutputConverter(IAnalogOutputs* aInterface) : mInterface(aInterface)
        {
            assert(NULL != aInterface);
        }

        // ===== IAnalogOutputs =============================================
        AnalogValue AnalogOutputConverter::AO_Get(Id aId)
        {
            assert(NULL != mInterface);

            return Revert(aId, mInterface->AO_Get(aId));
        }

        AnalogValue_Raw AnalogOutputConverter::AO_Get_Raw(Id aId)
        {
            assert(NULL != mInterface);

            return mInterface->AO_Get_Raw(aId);
        }

        void AnalogOutputConverter::AO_Write(Id aId, AnalogValue aValue)
        {
            assert(NULL != mInterface);

            mInterface->AO_Write(aId, Scale(aId, aValue));
        }

        void AnalogOutputConverter::AO_Write_Raw(Id aId, AnalogValue_Raw aValue)
        {
            assert(NULL != mInterface);

            mInterface->AO_Write_Raw(aId, aValue);
        }

    }
}
