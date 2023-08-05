
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/Test.h
// Status

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/DAQ/DigitalOutput.h>
#include <KMS/Embedded/IInterruptHandler.h>
#include <KMS/Embedded/USART.h>
#include <KMS/WOP/Link_USART.h>
#include <KMS/WOP/System.h>

namespace KMS
{
    namespace Embedded
    {

        class Test : public IInterruptHandler
        {

        public:

            const Callback<Test> ON_KEY;

            Test(USART* aUSART, const DAQ::DigitalOutput& aLED);

            int Run();

            // ===== IInterruptHandler ======================================
            virtual void OnInterrupt(uint8_t aIndex, uint8_t aLevel);

        private:

            DAQ::DigitalOutput mLED;
            WOP::Link_USART    mLink;
            WOP::System        mSystem;

        };

    }
}
