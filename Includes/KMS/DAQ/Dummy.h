
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/Dummy.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/IAnalogInputs.h>
#include <KMS/DAQ/IAnalogOutputs.h>
#include <KMS/DAQ/IDigitalInputs.h>
#include <KMS/DAQ/IDigitalOutputs.h>

namespace KMS
{
    namespace DAQ
    {

        class Dummy : public IAnalogInputs, public IAnalogOutputs, public IDigitalInputs, public IDigitalOutputs
        {

        public:

            Dummy();

            void AI_Simulate    (KMS::DAQ::Id aId, AnalogValue     aValue);
            void AI_Simulate_Raw(KMS::DAQ::Id aId, AnalogValue_Raw aValue);

            void DI_Simulate(KMS::DAQ::Id aId, bool aValue);

            // ===== IAnalogInputs ==========================================
            virtual AnalogValue     AI_Read    (KMS::DAQ::Id aId);
            virtual AnalogValue_Raw AI_Read_Raw(KMS::DAQ::Id aId);

            // ===== IAnalogOutputs =========================================
            virtual AnalogValue     AO_Get      (KMS::DAQ::Id aId);
            virtual AnalogValue_Raw AO_Get_Raw  (KMS::DAQ::Id aId);
            virtual void            AO_Write    (KMS::DAQ::Id aId, AnalogValue aValue);
            virtual void            AO_Write_Raw(KMS::DAQ::Id aId, AnalogValue_Raw aValue_Raw);

            // ===== IDigitalInputs =========================================
            virtual bool DI_Read(KMS::DAQ::Id aId);

            // ===== IDigitalOutputs ========================================
            virtual void DO_Clear(KMS::DAQ::Id aId);
            virtual bool DO_Get  (KMS::DAQ::Id aId);
            virtual void DO_Set  (KMS::DAQ::Id aId, bool aValue);

        private:

            AnalogValue     mAI_Value    [256];
            AnalogValue_Raw mAI_Value_Raw[256];
            AnalogValue     mAO_Value    [256];
            AnalogValue_Raw mAO_Value_Raw[256];
            bool            mDI_Value    [256];
            bool            mDO_Value    [256];
            
        };

    }
}
