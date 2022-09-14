
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/AnalogInputConverter.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/AnalogConverter.h>
#include <KMS/DAQ/IAnalogInputs.h>

namespace KMS
{
    namespace DAQ
    {

        class AnalogInputConverter : public AnalogConverter, public IAnalogInputs
        {

        public:

            AnalogInputConverter(IAnalogInputs* aInterface);

            // ===== IAnalogInputs ==========================================
            virtual AnalogValue     AI_Read    (Id aId);
            virtual AnalogValue_Raw AI_Read_Raw(Id aId);

        private:

            IAnalogInputs* mInterface;

        };

    }
}
