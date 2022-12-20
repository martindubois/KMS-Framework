
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/AnalogOutputConverter.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/AnalogConverter.h>
#include <KMS/DAQ/IAnalogOutputs.h>

namespace KMS
{
    namespace DAQ
    {

        class AnalogOutputConverter : public AnalogConverter, public IAnalogOutputs
        {

        public:

            AnalogOutputConverter(IAnalogOutputs* aInterface);

            // ===== IAnalogOutputs =========================================
            virtual AnalogValue     AO_Get      (Id aId) const;
            virtual AnalogValue_Raw AO_Get_Raw  (Id aId) const;
            virtual void            AO_Write    (Id aId, AnalogValue     aValue);
            virtual void            AO_Write_Raw(Id aId, AnalogValue_Raw aValue);

        private:

            IAnalogOutputs* mInterface;

        };

    }
}
