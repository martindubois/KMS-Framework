
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Master.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/UInt.h>
#include <KMS/Modbus/Modbus.h>

namespace KMS
{
    namespace Modbus
    {

        class Master : public DI::Dictionary
        {

        public:

            void SetDeviceAddress(DeviceAddress aDA);

            DeviceAddress GetLastException() const;

            virtual void Connect() = 0;

            virtual void Disconnect() = 0;

            // ===== Modbus functions ===========================================

            bool ReadCoil(Address aAddr);

            bool ReadDiscreteInput(Address aAddr);

            RegisterValue ReadHoldingRegister(Address aAddr);

            RegisterValue ReadInputRegister(Address aAddr);

            void WriteSingleCoil(Address aAddr, bool aValue);

            void WriteSingleRegister(Address aAddr, RegisterValue aValue);

            // ===== Configurable attributes ================================
            DI::UInt<uint8_t> mDeviceAddress;
            DI::UInt<uint8_t> mRetryCount;

        protected:

            Master();

            void VerifyDeviceAddress(const uint8_t* aData);

            void VerifyFunction(Function aFunction, const uint8_t* aData);

            // ===== Low level request type =================================

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

        private:

            NO_COPY(Master);

            Exception mLastException;

        };

    }
}
