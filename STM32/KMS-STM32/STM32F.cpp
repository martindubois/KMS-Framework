
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/KMS-STM32/STM32F.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/USART.h>
#include <KMS/STM/STM32F.h>

// ===== Local ==============================================================
#include "USART.h"

// Variables
// //////////////////////////////////////////////////////////////////////////

static GPIO_TypeDef * sGPIOs [6] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF };

static uint8_t sUSART_DMA[3] = { 3, 6, 1 };

static ::USART sUSART_Instances[3];

namespace KMS
{
    namespace STM
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        STM32F::STM32F() : mClock_Hz(8000000) {}

        void STM32F::Clock_Config()
        {
            RCC->CR &= ~ RCC_CR_PLLON;

            RCC->CFGR &= ~ (RCC_CFGR_PLLSRC_Msk | RCC_CFGR_PLLMUL_Msk);
            RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL8;

            RCC->CR |= RCC_CR_PLLON;
            while (0 == (RCC->CR & RCC_CR_PLLRDY));

            RCC->CFGR &= ~ RCC_CFGR_HPRE_Msk;
            RCC->CFGR |=   RCC_CFGR_HPRE_DIV1;

            RCC->CFGR &= ~ (RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk);
            RCC->CFGR |= RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1;

            RCC->CFGR &= ~ RCC_CFGR_SW_Msk;
            RCC->CFGR |=   RCC_CFGR_SW_PLL;

            mClock_Hz *= 4;
        }

        void STM32F::IO_SetMode(DAQ::Id aId, IO_Mode aMode)
        {
            unsigned int lBit  = aId % 16;
            unsigned int lPort = aId / 16;

            RCC->AHBENR |= 1 << (17 + lPort);

            GPIO_TypeDef* lGPIO = sGPIOs[lPort];

            lGPIO->ODR &= ~ (1 << lBit);

            lGPIO->MODER &= ~ (0x3 << (lBit * 2));
            lGPIO->MODER |=    0x1 << (lBit * 2);

            lGPIO->OTYPER |= 1 << lBit;

            lGPIO->OSPEEDR &= ~ (0x3 << (lBit * 2));
            lGPIO->OSPEEDR |=    0x1 << (lBit * 2);

            lGPIO->PUPDR &= ~ (0x3 << (lBit * 2));
        }

        Embedded::USART* STM32F::USART_Get(uint8_t aId, DAQ::Id aRx, DAQ::Id aTx)
        {
            // USART 1 Rx: PA10 PB7 PC4 PE1
            //         Tx: PA9 PB6 PC4 PE0

            // USART 2 Rx: PA3 PB4 PD6
            //         Tx: PA2 PB3 PD5

            // USART 3 Rx: PB9 PC11 PD9 PE_15
            //         Tx: PB8 PB10 PC10 PD8

            IO_SetAltFunc(aRx, 7);
            IO_SetAltFunc(aTx, 7);

            ::USART* lResult = sUSART_Instances + aId;

            uint8_t lDMA = sUSART_DMA[aId];

            switch (aId)
            {
            case 0:
                RCC->CFGR3 &= ~ RCC_CFGR3_USART1SW_Msk;
                RCC->CFGR3 |=   RCC_CFGR3_USART1SW_SYSCLK;
                break;
            case 1:
                RCC->CFGR3 &= ~ RCC_CFGR3_USART2SW_Msk;
                RCC->CFGR3 |=   RCC_CFGR3_USART2SW_SYSCLK;
                break;
            case 2:
                RCC->CFGR3 &= ~ RCC_CFGR3_USART3SW_Msk;
                RCC->CFGR3 |=   RCC_CFGR3_USART3SW_SYSCLK;
                break;
            }

            RCC->AHBENR |= 0x1; // DMAEN

            switch (aId)
            {
            case 0: RCC->APB2ENR |= 0x04000; break;
            case 1: RCC->APB1ENR |= 0x20000; break;
            case 2: RCC->APB1ENR |= 0x40000; break;
            }

            lResult->Init(aId, lDMA, mClock_Hz);

            return lResult;
        }

        // ===== DAQ::IDigitalOutputs =======================================

        void STM32F::DO_Clear(DAQ::Id aId)
        {
            sGPIOs[aId / 16]->BSRR = 0x10000 << (aId % 16);
        }

        bool STM32F::DO_Get(DAQ::Id aId) const
        {
            return 0 != (sGPIOs[aId / 16]->ODR & (1 << (aId % 16)));
        }

        void STM32F::DO_Set(DAQ::Id aId, bool aValue)
        {
            GPIO_TypeDef* lGPIO = sGPIOs[aId / 16];

            uint8_t lBit = aId % 16;

            if (aValue)
            {
                lGPIO->BSRR = 1 << lBit;
            }
            else
            {
                lGPIO->BSRR = 0x10000 << lBit;
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void STM32F::IO_SetAltFunc(DAQ::Id aId, unsigned int aAltFunc)
        {
            uint8_t lBit  = aId % 16;
            uint8_t lBi2  = lBit % 8;
            uint8_t lPort = aId / 16;
            uint8_t lReg  = lBit / 8;

            RCC->AHBENR |= 1 << (17 + lPort);

            GPIO_TypeDef* lGPIO = sGPIOs[lPort];

            lGPIO->MODER &= ~ (0x3 << (2 * lBit));
            lGPIO->MODER |=    0x2 << (2 * lBit);

            lGPIO->AFR[lReg] &= 0xf      << (4 * lBi2);
            lGPIO->AFR[lReg] |= aAltFunc << (4 * lBi2);
        }

    }
}

// Entry points
// //////////////////////////////////////////////////////////////////////////

extern "C"
{

    void DMA1_CH2_IRQHandler() { sUSART_Instances[2].Tx_OnInterrupt(); }
    void DMA1_CH4_IRQHandler() { sUSART_Instances[0].Tx_OnInterrupt(); }
    void DMA1_CH7_IRQHandler() { sUSART_Instances[1].Tx_OnInterrupt(); }

    void USART1_IRQHandler() { sUSART_Instances[0].Rx_OnInterrupt(); }
    void USART2_IRQHandler() { sUSART_Instances[1].Rx_OnInterrupt(); }
    void USART3_IRQHandler() { sUSART_Instances[2].Rx_OnInterrupt(); }

}