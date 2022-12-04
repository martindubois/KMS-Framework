
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
            : mLED(aLED)
            , mLink(&mSystem, aUSART)
            , mSystem(VERSION, 0x74736554, 0x00)
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

        // ===== IInterruptHandler ==========================================

        void Test::OnInterrupt(uint8_t, uint8_t aLevel)
        {
            // assert(NULL != mUSART);

            mSystem.AddTrace(aLevel ? "Key\r\n" : "KEY\r\n", 5);
        }

    }
}
