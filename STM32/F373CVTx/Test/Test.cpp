
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/Test/Test.cpp

// ===== C ==================================================================
#include <stdint.h>
#include <stdlib.h>

// ===== Includes ===========================================================
#include <KMS/Embedded/Test.h>

#include <KMS/STM/STM32F.h>

using namespace KMS;

// Static variables
// //////////////////////////////////////////////////////////////////////////

static STM::STM32F sProcessor;

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main()
{
    sProcessor.Clock_Config();

    sProcessor.IO_SetMode(KMS_STM_ID_PC(0), STM::STM32F::IO_Mode::DIGITAL_OUTPUT_OPEN_DRAIN);

    Embedded::USART* lUSART = sProcessor.USART_Get(1, KMS_STM_ID_PD(6), KMS_STM_ID_PD(5));

    Embedded::Test lTest(lUSART, DAQ::DigitalOutput(&sProcessor, KMS_STM_ID_PC(0)));

    sProcessor.IO_ConfigureInterrupt(KMS_STM_ID_PA(2), lTest.ON_KEY);

    return lTest.Run();
}
