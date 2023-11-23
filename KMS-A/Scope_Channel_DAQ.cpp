
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Scope_Channel_DAQ.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Scope/Channel_DAQ.h>

namespace KMS
{
    namespace Scope
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Channel_DAQ_AI::Channel_DAQ_AI(DAQ::AnalogInput* aAI) : mAI(aAI)
        {
            assert(nullptr != aAI);
        }

        Channel_DAQ_AO::Channel_DAQ_AO(DAQ::AnalogOutput* aAO) : mAO(aAO)
        {
            assert(nullptr != aAO);
        }

        Channel_DAQ_DI::Channel_DAQ_DI(DAQ::DigitalInput* aDI) : mDI(aDI)
        {
            assert(nullptr != aDI);
        }

        Channel_DAQ_DO::Channel_DAQ_DO(DAQ::DigitalOutput* aDO) : mDO(aDO)
        {
            assert(nullptr != aDO);
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Channel_DAQ_AI::Sample(unsigned int* aY_px, bool* aTrig)
        {
            assert(nullptr != mAI);

            SetSample(mAI->Read());

            Channel::Sample(aY_px, aTrig);
        }

        void Channel_DAQ_AO::Sample(unsigned int* aY_px, bool* aTrig)
        {
            assert(nullptr != mAO);

            SetSample(mAO->Get());

            Channel::Sample(aY_px, aTrig);
        }

        void Channel_DAQ_DI::Sample(unsigned int* aY_px, bool* aTrig)
        {
            assert(nullptr != mDI);

            SetSample(mDI->Read());

            Channel::Sample(aY_px, aTrig);
        }

        void Channel_DAQ_DO::Sample(unsigned int* aY_px, bool* aTrig)
        {
            assert(nullptr != mDO);

            SetSample(mDO->Get());

            Channel::Sample(aY_px, aTrig);
        }

    }
}
