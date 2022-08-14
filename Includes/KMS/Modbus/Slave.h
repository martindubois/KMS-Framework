
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurable.h>
#include <KMS/Modbus/Modbus.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/Msg/Sender.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave : public Cfg::Configurable, public Msg::IReceiver
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

            static const unsigned int MSG_ITERATE;

            void SetDeviceAddress(DeviceAddress aDA);

            virtual void Run();

            virtual void Stop();

            Msg::Sender mOnReadCoils;
            Msg::Sender mOnReadDiscreteInputs;
            Msg::Sender mOnReadHoldingRegisters;
            Msg::Sender mOnReadInputRegisters;
            Msg::Sender mOnWriteSingleCoil;
            Msg::Sender mOnWriteSingleRegister;

            virtual void Connect() = 0;

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

            // ===== Cfg::Configurable ======================================
            virtual void DisplayHelp(FILE* aOut) const;
            virtual bool SetAttribute(const char* aA, const char* aV);

        protected:

            Slave();

            DeviceAddress GetDeviceAddress() const;

            virtual unsigned int OnIterate() = 0;

            void OnRequest_A_Bit (Function aFunction, Address aStartAddr, uint16_t aQty, Msg::Sender* aSender);
            void OnRequest_A_Word(Function aFunction, Address aStartAddr, uint16_t aQty, Msg::Sender* aSender);

            void OnRequest_B(Function aFunction, Address aStartAddr, void* aData, Msg::Sender* aSender);

            virtual void SendException(Function aFunction, Exception aException) = 0;

            virtual void SendResponse_A(Function aFunction, uint8_t aSize_byte, const void* aData) = 0;
            virtual void SendResponse_B(Function aFunction, Address aAddress  , const void* aData) = 0;

        private:

            bool mStopped;

            // ===== Configurable attributes ================================
            DeviceAddress mDeviceAddress;

        };

    }
}
