
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave_Com.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Dev/IDevice.h>
#include <KMS/Modbus/Slave_Serial.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave_IDevice : public Slave_Serial
        {

        public:

            Slave_IDevice(Dev::IDevice* aDevice);

            // ===== Slave ==================================================
            virtual void Connect();

        protected:

            // ===== Slave_Serial ===========================================
            virtual void SendData(const uint8_t* aIn, unsigned int aInSize_byte);

            // ===== Slave ==================================================
            virtual unsigned int OnIterate();

        private:

            NO_COPY(Slave_IDevice);

            Dev::IDevice* mDevice;

        };

    }
}
