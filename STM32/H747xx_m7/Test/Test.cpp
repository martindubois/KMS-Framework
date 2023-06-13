
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/H747xx_m7/Test/Test.cpp

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
    SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

    if (FLASH_LATENCY_DEFAULT > (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY)))
    {
        MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)(FLASH_LATENCY_DEFAULT));
    }

    RCC->CR |= RCC_CR_HSION;

    RCC->CFGR = 0x00000000;

    RCC->CR &= 0xEAF6ED7FU;

    if (FLASH_LATENCY_DEFAULT < (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY)))
    {
      MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, (uint32_t)(FLASH_LATENCY_DEFAULT));
    }

    RCC->D1CFGR = 0x00000000;
    RCC->D2CFGR = 0x00000000;
    RCC->D3CFGR = 0x00000000;

    RCC->PLLCKSELR = 0x02020200;

    RCC->PLLCFGR = 0x01FF0000;
    RCC->PLL1DIVR = 0x01010280;
    RCC->PLL1FRACR = 0x00000000;

    RCC->PLL2DIVR = 0x01010280;

    RCC->PLL2FRACR = 0x00000000;
    RCC->PLL3DIVR = 0x01010280;

    RCC->PLL3FRACR = 0x00000000;

    RCC->CR &= 0xFFFBFFFFU;

    RCC->CIER = 0x00000000;

    EXTI_D2->EMR3 |= 0x4000UL;

    if ((DBGMCU->IDCODE & 0xFFFF0000U) < 0x20000000U)
    {
      *((__IO uint32_t*)0x51008108) = 0x000000001U;
    }

    FMC_Bank1_R->BTCR[0] = 0x000030D2;
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
