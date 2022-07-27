
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/IDigitalOutputs.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DAQ.h>

namespace KMS
{
    namespace DAQ
    {

        class IDigitalOutputs
        {

        public:

            virtual void DO_Clear(Id aId) = 0;

            virtual bool DO_Get(Id aId) const = 0;

            virtual void DO_Set(Id aId, bool aValue = true) = 0;

        };

    }
}
