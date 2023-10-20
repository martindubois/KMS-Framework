
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/STM/STM32F.h
// Library   KMS-STM32

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== STM ================================================================
#include <stm32f3xx.h>

// ===== Includes ===========================================================
#include <KMS/DAQ/IDigitalInputs.h>
#include <KMS/DAQ/IDigitalOutputs.h>
#include <KMS/Embedded/IInterruptHandler.h>
#include <KMS/Embedded/SPI.h>
#include <KMS/Embedded/USART.h>

#define KMS_STM_ID_PA(I) (0 * 16 + (I))
#define KMS_STM_ID_PB(I) (1 * 16 + (I))
#define KMS_STM_ID_PC(I) (2 * 16 + (I))
#define KMS_STM_ID_PD(I) (3 * 16 + (I))
#define KMS_STM_ID_PE(I) (4 * 16 + (I))
#define KMS_STM_ID_PF(I) (5 * 16 + (I))

namespace KMS
{
    namespace STM
    {

        class STM32F final : public DAQ::IDigitalInputs, public DAQ::IDigitalOutputs
        {

        public:

            enum class IO_Mode
            {
                DIGITAL_INPUT = 0,
                DIGITAL_OUTPUT_OPEN_DRAIN,
                DIGITAL_OUTPUT_PUSH_PULL,
            };

            // Level: INIT ONCE
            STM32F();

            // Configure clock using HSI and PLL
            // Level: INIT ONCE
            void Clock_32_MHz();
            void Clock_48_MHz();
            void Clock_64_MHz();

            // Level: INIT
            void IO_ConfigureInterrupt(DAQ::Id aId, Embedded::IInterruptHandler* aHandler);

            // aId    See KMS_STM_ID_P...
            // aMode  See IO_Mode
            // Level: INIT
            void IO_SetMode(DAQ::Id aId, IO_Mode aMode);

            // aId  Zero based SPI index
            // aRx  Rx pin, see KMS_STM_ID_P...
            // aTx  Tx pin, see KMS_STM_ID_P...
            // Level: INIT
            Embedded::SPI* SPI_Get(uint8_t aId, DAQ::Id aRx, DAQ::Id aTx, DAQ::Id aClk);

            // The UART is configured for 115200 bps 8N1
            // aId  Zero based USART index
            // aRx  Rx pin, see KMS_STM_ID_P...
            // aTx  Tx pin, see KMS_STM_ID_P...
            // Level: INIT
            Embedded::USART* USART_Get(uint8_t aId, DAQ::Id aRx, DAQ::Id aTx);

            // ==== DAQ::IDigitalInputs =====================================
            virtual bool DI_Read(DAQ::Id aId);

            // ==== DAQ::IDigitalOutputs ====================================
            virtual void DO_Clear(DAQ::Id aId);
            virtual bool DO_Get  (DAQ::Id aId) const;
            virtual void DO_Set  (DAQ::Id aId, bool aValue = true);

        private:

            void IO_SetAltFunc(DAQ::Id aId, unsigned int aAltFunc);

            uint32_t mClock_Hz;

        };

    }
}
