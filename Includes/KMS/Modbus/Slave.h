
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Di/Dictionary.h>
#include <KMS/DI/UInt.h>
#include <KMS/Modbus/Modbus.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/Msg/Destination.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave : public DI::Dictionary, public Msg::IReceiver
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

            const Msg::Destination ON_ITERATE;

            virtual void Run();

            virtual void Stop();

            Msg::Destination mOnReadCoils;
            Msg::Destination mOnReadDiscreteInputs;
            Msg::Destination mOnReadHoldingRegisters;
            Msg::Destination mOnReadInputRegisters;
            Msg::Destination mOnWriteSingleCoil;
            Msg::Destination mOnWriteSingleRegister;

            virtual void Connect() = 0;

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        protected:

            Slave();

            DeviceAddress GetDeviceAddress() const;

            virtual unsigned int OnIterate() = 0;

            void OnRequest_A_Bit (Function aFunction, Address aStartAddr, uint16_t aQty, Msg::Destination* aDst);
            void OnRequest_A_Word(Function aFunction, Address aStartAddr, uint16_t aQty, Msg::Destination* aDst);

            void OnRequest_B(Function aFunction, Address aStartAddr, void* aData, Msg::Destination* aDst);

            virtual void SendException(Function aFunction, Exception aException) = 0;

            virtual void SendResponse_A(Function aFunction, uint8_t aSize_byte, const void* aData) = 0;
            virtual void SendResponse_B(Function aFunction, Address aAddress  , const void* aData) = 0;

        private:

            bool mStopped;

            // ===== Configurable attributes ================================
            DI::UInt<uint8_t> mDeviceAddress;

        };

    }
}
