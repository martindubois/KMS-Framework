
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Embedded_Test.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/WorkLoop.h>

#include <KMS/Embedded/Test.h>

// ===== Local ==============================================================
#include "../../../Common/Version.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_KEY (1)

namespace KMS
{
    namespace Embedded
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Test::Test(Embedded::USART* aUSART, const DAQ::DigitalOutput& aLED)
            : ON_KEY(this, MSG_KEY)
            , mLED(aLED)
            , mLink(&mReceiver, &mSender, aUSART)
            , mReceiver(&mSystem, NULL, 0)
            , mSender(&mSystem, NULL, 0)
            , mSystem(VERSION)
        {
            // assert(NULL != aUSART);
        }

        int Test::Run()
        {
            USART* lUSART = mLink.GetUSART();
            // assert(NULL != lUSART);

            lUSART->Rx_Enable();

            Embedded::WorkLoop lWorkLoop;

            lWorkLoop.AddItem(&mLink);
            lWorkLoop.AddItem(lUSART);

            mLED.Set(false);

            lWorkLoop.Run();

            return 0;
        }

        // ===== Msg::IReceiver =============================================

        unsigned int Test::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

            switch (aCode)
            {
            case MSG_KEY: lResult = OnKey(); break;
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int Test::OnKey()
        {
            // assert(NULL != mUSART);

            mSystem.AddTrace("Key\r\n", 5);

            return 0;
        }

    }
}
