
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/KMS-STM32/STM32F.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/USART.h>
#include <KMS/STM/STM32F.h>

// ===== Local ==============================================================
#include "SPI.h"
#include "USART.h"

using namespace KMS;

// Data types
// //////////////////////////////////////////////////////////////////////////

class InterruptInfo
{

public:

    volatile uint32_t          * mDataReg;
    Embedded::IInterruptHandler* mHandler;

};

// Variables
// //////////////////////////////////////////////////////////////////////////

#define GPIO_QTY  (6)
#define SPI_QTY   (3)
#define USART_QTY (3)

static GPIO_TypeDef * sGPIOs[GPIO_QTY] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF };

static uint8_t sUSART_DMA[USART_QTY] = { 3, 6, 1 };

// Putting the USART instances in a static array allow to don't leak any
// information about the internal USART class. Any way, creating more than 1
// instances of STM32F class make no sense.
static ::USART sUSART_Instances[USART_QTY];
static ::SPI   sSPI_Instances  [SPI_QTY  ];

static InterruptInfo sInterrupts[16];

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void OnInterrupt(uint8_t aIndex);

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
            RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL16;

            RCC->CR |= RCC_CR_PLLON;
            while (0 == (RCC->CR & RCC_CR_PLLRDY));

            RCC->CFGR &= ~ RCC_CFGR_HPRE_Msk;
            RCC->CFGR |=   RCC_CFGR_HPRE_DIV1;

            RCC->CFGR &= ~ (RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk);
            RCC->CFGR |= RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV2;

            RCC->CFGR &= ~ RCC_CFGR_SW_Msk;
            RCC->CFGR |=   RCC_CFGR_SW_PLL;

            mClock_Hz *= 8;
        }

        static const IRQn_Type EXTI_IRQ[16] =
        {
            EXTI0_IRQn  , EXTI1_IRQn  , EXTI2_TSC_IRQn, EXTI3_IRQn    , EXTI4_IRQn    , EXTI9_5_IRQn  , EXTI9_5_IRQn  , EXTI9_5_IRQn  ,
            EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn,
        };

        void STM32F::IO_ConfigureInterrupt(DAQ::Id aId, Embedded::IInterruptHandler* aHandler)
        {
            uint8_t  lBit  = aId % 16;
            uint32_t lMask = 1 << lBit;
            uint32_t lPort = aId / 16;
            uint8_t  lReg  = lBit / 4;
            uint8_t  lPos  = 4 * (lBit % 4);

            RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

            sInterrupts[lBit].mDataReg = &sGPIOs[lPort]->IDR;
            sInterrupts[lBit].mHandler = aHandler;

            SYSCFG->EXTICR[lReg] &= ~ (SYSCFG_EXTICR1_EXTI0_Msk << lPos);
            SYSCFG->EXTICR[lReg] |= lPort << lPos;

            EXTI->IMR  |= lMask;
            EXTI->EMR  |= lMask;
            EXTI->RTSR |= lMask;
            EXTI->FTSR |= lMask;

            NVIC_EnableIRQ(EXTI_IRQ[lBit]);
        }

        static uint32_t GPIO_MODER_MASK      = 0x3;
        static uint32_t GPIO_MODER_VALUES[3] = { 0x0, 0x1, 0x1 };

        // TODO Let user choose pull up, pull down and speed

        void STM32F::IO_SetMode(DAQ::Id aId, IO_Mode aMode)
        {
            unsigned int lBit  = aId % 16;
            unsigned int lPort = aId / 16;

            // assert(GPIO_QTY > lPort);

            RCC->AHBENR |= 1 << (17 + lPort);

            GPIO_TypeDef* lGPIO = sGPIOs[lPort];

            lGPIO->ODR &= ~ (1 << lBit);

            lGPIO->MODER &= ~ (GPIO_MODER_MASK                                     << (lBit * 2));
            lGPIO->MODER |=    GPIO_MODER_VALUES[static_cast<unsigned int>(aMode)] << (lBit * 2);

            switch (aMode)
            {
            case IO_Mode::DIGITAL_INPUT: break;

            case IO_Mode::DIGITAL_OUTPUT_OPEN_DRAIN: lGPIO->OTYPER |=    1 << lBit ; break;
            case IO_Mode::DIGITAL_OUTPUT_PUSH_PULL : lGPIO->OTYPER &= ~ (1 << lBit); break;
            }

            lGPIO->OSPEEDR &= ~ (0x3 << (lBit * 2));
            lGPIO->OSPEEDR |=    0x1 << (lBit * 2);

            lGPIO->PUPDR &= ~ (0x3 << (lBit * 2));
        }

        // SPI 1 Rx:  PA6 5  PA13 6  PB4 5  PC8 5
        //       Tx:  PA7 5  PB0  5  PB5 5  PC9 5  PF6 5
        //       Clk: PA5 5  PA12 6  PB3 5  PC7 5
        // SPI 2 Rx:  PA9  5  PB14 5  PC2  5  PD3 5
        //       Tx:  PA10 5  PB16 5  PC3  5  PD4 5
        //       Clk: PA8  5  PB8  5  PB10 5  PD7 5  PD8 5
        // SPI 3 Rx:  PA2 6  PB4 6  PC11 6
        //       Tx:  PA3 6  PB5 6  PC12 6
        //       Clk: PA1 6  PB3 6  PC10 6
        Embedded::SPI* STM32F::SPI_Get(uint8_t aId, DAQ::Id aRx, DAQ::Id aTx, DAQ::Id aClk)
        {
            uint8_t lAltClk = 5;
            uint8_t lAltRx  = 5;
            uint8_t lAltTx  = 5;

            switch (aId)
            {
            case 0:
                switch (aRx)
                {
                case KMS_STM_ID_PA(13): lAltRx = 6; break;
                }
                switch (aClk)
                {
                case KMS_STM_ID_PA(12): lAltClk = 6; break;
                }
                break;
            case 2: lAltClk = 6; lAltRx = 6; lAltTx = 6; break;
            }

            IO_SetAltFunc(aClk, lAltClk);
            IO_SetAltFunc(aRx , lAltRx );
            IO_SetAltFunc(aTx , lAltTx );

            ::SPI* lResult = sSPI_Instances + aId;

            lResult->Init(aId);

            return lResult;
        }

        // All the pin we can use for USART1, 2 and 3 must be configured
        // using the alternate function 7.
        // USART 1 Rx: PA10 PB7 PC4 PE1
        //         Tx: PA9 PB6 PC4 PE0
        // USART 2 Rx: PA3 PB4 PD6
        //         Tx: PA2 PB3 PD5
        // USART 3 Rx: PB9 PC11 PD9 PE_15
        //         Tx: PB8 PB10 PC10 PD8
        Embedded::USART* STM32F::USART_Get(uint8_t aId, DAQ::Id aRx, DAQ::Id aTx)
        {
            // assert(USART_QTY > aId);

            IO_SetAltFunc(aRx, 7);
            IO_SetAltFunc(aTx, 7);

            ::USART* lResult = sUSART_Instances + aId;

            uint8_t lDMA = sUSART_DMA[aId];

            // TODO Simplify by using tables
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

        // ===== DAQ::IDigitalInputs ========================================

        bool STM32F::DI_Read(DAQ::Id aId)
        {
            return 0 != (sGPIOs[aId / 16]->IDR & (1 << (aId % 16)));
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

            // assert(GPIO_QTY > lPort);

            RCC->AHBENR |= 1 << (17 + lPort);

            GPIO_TypeDef* lGPIO = sGPIOs[lPort];

            lGPIO->MODER &= ~ (0x3 << (2 * lBit));
            lGPIO->MODER |=    0x2 << (2 * lBit);

            lGPIO->AFR[lReg] &= ~ (0xf   << (4 * lBi2));
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

    void EXTI0_IRQHandler   () { OnInterrupt(0); }
    void EXTI1_IRQHandler   () { OnInterrupt(1); }
    void EXTI2_TS_IRQHandler() { OnInterrupt(2); }
    void EXTI3_IRQHandler   () { OnInterrupt(3); }
    void EXTI4_IRQHandler   () { OnInterrupt(4); }

    void EXTI5_9_IRQHandler  ()
    {
        for (uint8_t i = 5; i <= 9; i++)
        {
            if (0 != (EXTI->PR & (1 << i)))
            {
                OnInterrupt(i);
            }
        }
    }

    void EXTI15_10_IRQHandler()
    {
        for (uint8_t i = 10; i <= 15; i++)
        {
            if (0 != (EXTI->PR & (1 << i)))
            {
                OnInterrupt(i);
            }
        }
    }

    void SPI1_IRQHandler() { sSPI_Instances[0].OnInterrupt(); }
    void SPI2_IRQHandler() { sSPI_Instances[1].OnInterrupt(); }
    void SPI3_IRQHandler() { sSPI_Instances[2].OnInterrupt(); }

    void USART1_IRQHandler() { sUSART_Instances[0].Rx_OnInterrupt(); }
    void USART2_IRQHandler() { sUSART_Instances[1].Rx_OnInterrupt(); }
    void USART3_IRQHandler() { sUSART_Instances[2].Rx_OnInterrupt(); }

}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void OnInterrupt(uint8_t aIndex)
{
    uint32_t lBit   = 1 << aIndex;
    uint8_t  lLevel = 0 == (*sInterrupts[aIndex].mDataReg & lBit) ? 0 : 1;

    sInterrupts[aIndex].mHandler->OnInterrupt(aIndex, lLevel);

    EXTI->PR = lBit;
}
