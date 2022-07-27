
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/IAnalogOutputs.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAQ.h>

namespace KMS
{
    namespace DAQ
    {

        class IAnalogOutputs
        {

        public:

            virtual AnalogValue AO_Get(Id aId) const = 0;

            virtual AnalogValue_Raw AO_Get_Raw(Id aId) const = 0;

            virtual void AO_Write(Id aId, AnalogValue aValue) = 0;

            virtual void AO_Write_Raw(Id aId, AnalogValue_Raw aValue) = 0;

        };

    }
}
