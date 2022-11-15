
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/KMS-STM32/STM32F.cpp

#include "Component.h"

// ===== STM ================================================================
#include <stm32f3xx.h>

// ===== Includes ===========================================================
#include <KMS/STM/STM32F.h>

// Variables
// //////////////////////////////////////////////////////////////////////////

static GPIO_TypeDef* sGPIOs[6] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF };

namespace KMS
{
    namespace STM
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const DAQ::Id STM32F::ID_PC0 = 2 * 16 + 0;

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

        // ===== DAQ::IDigitalOutputs =======================================

        void STM32F::DO_Clear(DAQ::Id aId)
        {
            GPIO_TypeDef* lGPIO = sGPIOs[aId / 16];

            lGPIO->ODR &= ~ (1 << (aId % 16));
        }

        bool STM32F::DO_Get(DAQ::Id aId) const
        {
            GPIO_TypeDef* lGPIO = sGPIOs[aId / 16];

            return 0 != (lGPIO->ODR & (1 << (aId % 16)));
        }

        void STM32F::DO_Set(DAQ::Id aId, bool aValue)
        {
            GPIO_TypeDef* lGPIO = sGPIOs[aId / 16];

            if (aValue)
            {
                lGPIO->ODR |= 1 << (aId % 16);
            }
            else
            {
                lGPIO->ODR &= ~ (1 << (aId % 16));
            }
        }

    }
}
