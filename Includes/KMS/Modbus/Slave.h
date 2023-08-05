
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave.h
// Status
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Modbus/Modbus.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave
        {

        public:

            class MsgData
            {

            public:

                uint8_t * mBuffer;
                Exception mException;
                uint16_t  mQty;
                Address   mStartAddr;

            };

            void SetDeviceAddress(DeviceAddress aDA);

            virtual void Run();

            virtual void Stop();

            Callback_Ptr mOnReadCoils;
            Callback_Ptr mOnReadDiscreteInputs;
            Callback_Ptr mOnReadHoldingRegisters;
            Callback_Ptr mOnReadInputRegisters;
            Callback_Ptr mOnWriteSingleCoil;
            Callback_Ptr mOnWriteSingleRegister;

            virtual bool Connect() = 0;

            // ===== Confiurable attributes - See Slave_Cfg =================
            uint8_t mDeviceAddress;

        protected:

            Slave();

            DeviceAddress GetDeviceAddress() const;

            virtual unsigned int OnIterate() = 0;

            void OnRequest_A_Bit (Function aFunction, Address aStartAddr, uint16_t aQty, Callback_Ptr* aDst);
            void OnRequest_A_Word(Function aFunction, Address aStartAddr, uint16_t aQty, Callback_Ptr* aDst);

            void OnRequest_B(Function aFunction, Address aStartAddr, void* aData, Callback_Ptr* aDst);

            virtual void SendException(Function aFunction, Exception aException) = 0;

            virtual void SendResponse_A(Function aFunction, uint8_t aSize_byte, const void* aData) = 0;
            virtual void SendResponse_B(Function aFunction, Address aAddress  , const void* aData) = 0;

        private:

            NO_COPY(Slave);

            bool mStopped;

        };

    }
}
