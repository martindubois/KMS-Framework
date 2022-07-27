
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/IAnalogInputs.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAQ.h>

namespace KMS
{
    namespace DAQ
    {

        class IAnalogInputs
        {

        public:

            virtual AnalogValue AI_Read(Id aId) = 0;

            virtual AnalogValue_Raw AI_Read_Raw(Id aId) = 0;

        };

    }
}
