
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/Test.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DAQ/DigitalOutput.h>
#include <KMS/Embedded/USART.h>
#include <KMS/Msg/Destination.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/WOP/Link_USART.h>
#include <KMS/WOP/Receiver.h>
#include <KMS/WOP/Sender.h>
#include <KMS/WOP/System.h>

namespace KMS
{
    namespace Embedded
    {

        class Test : public Msg::IReceiver
        {

        public:

            const Msg::Destination ON_KEY;

            Test(USART* aUSART, const DAQ::DigitalOutput& aLED);

            int Run();

            // ===== Msg::IReceiver =================================================
            unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        private:

            // ===== Events =========================================================
            unsigned int OnKey();

            DAQ::DigitalOutput mLED;
            WOP::Link_USART    mLink;
            WOP::Receiver      mReceiver;
            WOP::Sender        mSender;
            WOP::System        mSystem;

        };

    }
}
