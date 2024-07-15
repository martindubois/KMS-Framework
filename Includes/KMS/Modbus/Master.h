
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Master.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Modbus/Modbus.h>
#include <KMS/Stream/IStream.h>

namespace KMS
{
    namespace Modbus
    {

        class Master
        {

        public:

            static const uint8_t DEVICE_ADDRESS_DEFAULT;
            static const uint8_t RETRY_COUNT_DEFAULT;

            uint8_t mDeviceAddress;
            uint8_t mRetryCount;

            void SetDeviceAddress(DeviceAddress aDA);

            Exception GetLastException() const;

            virtual bool Connect();

            virtual void Disconnect();

            // ===== Modbus functions ===========================================

            bool ReadCoil(Address aAddr, bool* aOut);

            bool ReadDiscreteInput(Address aAddr, bool* aOut);

            bool ReadHoldingRegister(Address aAddr, RegisterValue* aOut);

            bool ReadInputRegister(Address aAddr, RegisterValue* aOut);

            bool WriteSingleCoil(Address aAddr, bool aValue);

            bool WriteSingleRegister(Address aAddr, RegisterValue aValue);

        protected:

            static const unsigned int ERROR_BAD_CRC;
            static const unsigned int ERROR_BUFFER_TOO_SMALL;
            static const unsigned int ERROR_DEVICE_ADDRESS;
            static const unsigned int ERROR_FUNCTION;
            static const unsigned int ERROR_READ;
            static const unsigned int ERROR_SEND;

            Master(Stream::IStream* aStream);

            bool VerifyDeviceAddress(const uint8_t* aData);

            bool VerifyFunction(Function aFunction, const uint8_t* aData);

            // ===== Low level request type =================================
            // Return  ERROR_...

            // aFunction READ_COILS, READ_DISCRETE_INPUTS,
            //           READ_HOLDING_REGISTERS, READ_INPUT_REGISTERS,
            //           READ_WRITE_MULTIPLE_REGISTERS
            virtual unsigned int Request_A(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte) = 0;

            // aFunction     WRITE_SINGLE_COIL, WRITE_SINGLE_REGISTER
            // aOutSize_byte The method uses this value to know how much data
            //               to wait for.
            virtual unsigned int Request_B(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte) = 0;

            // aFunction     GET_COMM_EVENT_COUNTER, READ_EXCEPTION_STATUS
            // aOutSize_byte The method uses this value to know how much data
            //               to wait for.
            virtual unsigned int Request_C(Function aFunction, void* aOut, unsigned int aOutSize_byte) = 0;

            // aFunction     DIAGNOSTIC, MASK_WRITE_REGISTER
            // aOutSize_byte The method uses this value to know how much data
            //               to wait for.
            virtual unsigned int Request_D(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte) = 0;

            // aFunction     GET_COMM_EVENT_LOG, REPORT_SERVER_ID
            virtual unsigned int Request_E(Function aFunction, void* aOut, unsigned int aOutSize_byte) = 0;

            // aFunction     WRITE_MULTIPLE_COILS, WRITE_MULTIPLE_REGISTERS
            virtual void Request_F(Function aFunction, uint16_t aAddr, uint16_t aCount, const void* aIn, unsigned int aInSize_byte) = 0;

            // aFunction     READ_FIFO_QUEUE
            virtual unsigned int Request_G(Function aFunction, const void* aIn, unsigned int aSize_byte, void* aOut, unsigned int aOutSize_byte) = 0;

            Stream::IStream* mStream;

        private:

            NO_COPY(Master);

            Exception mLastException;

        };

    }
}
