
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave_USART.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Embedded/USART.h>
#include <KMS/Embedded/WorkItem.h>
#include <KMS/Modbus/Slave_Serial.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave_USART : public Slave_Serial, public Embedded::WorkItem
        {

        public:

            Slave_USART(KMS::Embedded::USART* aUSART);

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        protected:

            // ===== Slave_Serial ===========================================
            virtual void SendData(const uint8_t* aIn, unsigned int aInSize_byte);

            // ===== Embedded::WorkItem =====================================
            virtual void Work();

        private:

            NO_COPY(Slave_USART);

            unsigned int OnRxByte(void* aData);

            const Msg::Destination ON_RX_BYTE;

            Embedded::USART* mUSART;

        };

    }
}
