
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/STM/STM32F.h
// Library   KMS-STM32

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== Includes ===========================================================
#include <KMS/DAQ/IDigitalOutputs.h>
#include <KMS/Embedded/USART.h>
#include <KMS/Msg/IReceiver.h>

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

        class STM32F : public DAQ::IDigitalOutputs
        {

        public:

            // TODO DIGITAL_INPUT, DIGITAL_OUTPUT_PUSH_PULL
            enum class IO_Mode
            {
                DIGITAL_OUTPUT_OPEN_DRAIN,
            };

            STM32F();

            // Configure a 32 MHz clock using HSI and PLL
            void Clock_Config();

            void IO_ConfigureInterrupt(DAQ::Id aId, Msg::IReceiver* aReceiver, unsigned int aCode);

            // aId    See KMS_STM_ID_P...
            // aMode
            void IO_SetMode(DAQ::Id aId, IO_Mode aMode);

            // aId  Zero based USART index
            // aRx  Rx pin, see KMS_STM_ID_P...
            // aTx  Tx pin, see KMS_STM_ID_P...
            // The UART is configured for 115200 bps 8N1
            Embedded::USART* USART_Get(uint8_t aId, DAQ::Id aRx, DAQ::Id aTx);

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
