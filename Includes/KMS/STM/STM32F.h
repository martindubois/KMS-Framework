
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/STM/STM32F.h
// Library   KMS-STM32

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== Includes ===========================================================
#include <KMS/DAQ/IDigitalOutputs.h>

namespace KMS
{
    namespace STM
    {

        class STM32F : public KMS::DAQ::IDigitalOutputs
        {

        public:

            enum class IO_Mode
            {
                DIGITAL_OUTPUT,
            };

            static const DAQ::Id ID_PC0;

            void IO_SetMode(DAQ::Id aId, IO_Mode aMode);

            // ==== KMS::DAQ::IDigitalOutputs ===============================
            virtual void DO_Clear(DAQ::Id aId);
            virtual bool DO_Get  (DAQ::Id aId) const;
            virtual void DO_Set  (DAQ::Id aId, bool aValue = true);

        };

    }
}
