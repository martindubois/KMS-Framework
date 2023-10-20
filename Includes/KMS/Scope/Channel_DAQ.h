
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Scope/Channel_DAQ.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/AnalogInput.h>
#include <KMS/DAQ/AnalogOutput.h>
#include <KMS/DAQ/DigitalInput.h>
#include <KMS/DAQ/DigitalOutput.h>
#include <KMS/Scope/Channel.h>

namespace KMS
{
    namespace Scope
    {

        class Channel_DAQ_AI final : public Channel
        {

        public:

            Channel_DAQ_AI(DAQ::AnalogInput* aAI);

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);

        private:

            DAQ::AnalogInput* mAI;

        };

        class Channel_DAQ_AO final : public Channel
        {

        public:

            Channel_DAQ_AO(DAQ::AnalogOutput* aAO);

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);

        private:

            DAQ::AnalogOutput* mAO;

        };

        class Channel_DAQ_DI final : public Channel
        {

        public:

            Channel_DAQ_DI(DAQ::DigitalInput* aAI);

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);

        private:

            DAQ::DigitalInput* mDI;

        };

        class Channel_DAQ_DO final : public Channel
        {

        public:

            Channel_DAQ_DO(DAQ::DigitalOutput* aAI);

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);

        private:

            DAQ::DigitalOutput* mDO;

        };

    }
}
