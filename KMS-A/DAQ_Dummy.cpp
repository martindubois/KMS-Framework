
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DAC_Dummy.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DAQ/Dummy.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Dummy::Dummy()
        {
            memset(&mAO_Value    , 0, sizeof(mAO_Value));
            memset(&mAO_Value_Raw, 0, sizeof(mAO_Value_Raw));
            memset(&mDO_Value    , 0, sizeof(mAO_Value));
        }

        void Dummy::AI_Simulate    (Id aId, AnalogValue     aValue) { mAI_Value    [aId] = aValue; }
        void Dummy::AI_Simulate_Raw(Id aId, AnalogValue_Raw aValue) { mAI_Value_Raw[aId] = aValue; };

        void Dummy::DI_Simulate(Id aId, bool aValue) { mDI_Value[aId] = aValue; };

        // ===== IAnalogInputs ==============================================
        AnalogValue     Dummy::AI_Read    (Id) { return 0.0; }
        AnalogValue_Raw Dummy::AI_Read_Raw(Id) { return 0; }

        // ===== IAnalogOutputs =========================================
        AnalogValue     Dummy::AO_Get      (Id aId) { return mAO_Value    [aId]; }
        AnalogValue_Raw Dummy::AO_Get_Raw  (Id aId) { return mAO_Value_Raw[aId]; }
        void            Dummy::AO_Write    (Id aId, AnalogValue     aValue) { mAO_Value    [aId] = aValue; }
        void            Dummy::AO_Write_Raw(Id aId, AnalogValue_Raw aValue) { mAO_Value_Raw[aId] = aValue; }

        // ===== IDigitalInputs =========================================
        bool Dummy::DI_Read(Id) { return false; }

        // ===== IDigitalOutputs ========================================
        void Dummy::DO_Clear(Id aId) { mDO_Value[aId] = false; }
        bool Dummy::DO_Get  (Id aId) { return mDO_Value[aId]; }
        void Dummy::DO_Set  (Id aId, bool aValue) { mDO_Value[aId] = aValue; }


    }
}
