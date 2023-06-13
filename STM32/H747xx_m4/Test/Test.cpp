
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/H747xx_m4/Test/Test.cpp

#include <KMS/Base.h>

// ===== C ==================================================================
#include <stdint.h>
#include <stdlib.h>

// ===== STM ================================================================
#include <stm32h7xx.h>

// ===== Includes ===========================================================
#include <KMS/Embedded/Test.h>

#include <KMS/STM/STM32H.h>

using namespace KMS;

// Static variables
// //////////////////////////////////////////////////////////////////////////

static STM::STM32H sProcessor;

// Entry point
// //////////////////////////////////////////////////////////////////////////

void SystemInit()
{
    SCB->CPACR |= ((3UL << (10*2))|(3UL << (11*2)));  /* set CP10 and CP11 Full Access */

    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
}

int main()
{
    // TODO sProcessor.Clock_64_MHz();

    // The selected IOs fit the STM32373C-EVAL board.

    // TODO sProcessor.IO_SetMode(KMS_STM_ID_PC(0), STM::STM32F::IO_Mode::DIGITAL_OUTPUT_OPEN_DRAIN);

    // TODO Embedded::USART* lUSART = sProcessor.USART_Get(1, KMS_STM_ID_PD(6), KMS_STM_ID_PD(5));

    // TODO Embedded::Test lTest(lUSART, DAQ::DigitalOutput(&sProcessor, KMS_STM_ID_PC(0)));

    // TODO sProcessor.IO_ConfigureInterrupt(KMS_STM_ID_PA(2), &lTest);

    // TODO return lTest.Run();
}
