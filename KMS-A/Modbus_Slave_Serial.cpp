
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Slave_Serial.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/Slave_Serial.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

//   --> DEV_ADDR <--------------------------------------+
//        |                                              |
//        +--> FUNCTION                                  |
//              |    |                                   |
// OUT_ADDR_H --+    +--> START_ADDR_H                   |
//  |                      |                             |
//  +--> OUT_ADDR_L        +--> START_ADDR_L             |
//        |                      |                       |
//        +--> VALUE_H           +--> QTY_H              |
//              |                      |                 |
//              +--> VALUE_L           +--> QTY_L        |
//                    |                      |           |
//                    +----------------------+==> CRC_L  |
//                                                 |     |
//                                                 +--> CRC_H
#define STATE_CRC_H        ( 0)
#define STATE_CRC_L        ( 1)
#define STATE_DEV_ADDR     ( 2)
#define STATE_FUNCTION     ( 3)
#define STATE_OUT_ADDR_H   ( 4)
#define STATE_OUT_ADDR_L   ( 5)
#define STATE_QTY_H        ( 6)
#define STATE_QTY_L        ( 7)
#define STATE_START_ADDR_H ( 8)
#define STATE_START_ADDR_L ( 9)
#define STATE_VALUE_H      (10)
#define STATE_VALUE_L      (11)

namespace KMS
{
    namespace Modbus
    {

        // Protected
        // //////////////////////////////////////////////////////////////////

        Slave_Serial::Slave_Serial() { Reset(); }

        void Slave_Serial::OnByte(uint8_t aByte)
        {
             switch (mState)
             {
             case STATE_CRC_H:
                 mReceivedCRC |= aByte << 8;
                 if (mDeviceAddress == mTarget)
                 {
                     OnRequest();
                 }
                 else
                 {
                     Reset();
                 }
                 return;
             case STATE_CRC_L: mReceivedCRC =  aByte; mState = STATE_CRC_H; return;

             case STATE_DEV_ADDR:
                 mState  = STATE_FUNCTION;
                 mTarget = aByte;
                 break;

             case STATE_FUNCTION:
                 mFunction = static_cast<Function>(aByte);
                 switch (mFunction)
                 {
                 case Function::READ_COILS            :
                 case Function::READ_DISCRETE_INPUTS  :
                 case Function::READ_HOLDING_REGISTERS:
                 case Function::READ_INPUT_REGISTERS  : mState = STATE_START_ADDR_H; break;

                 case Function::WRITE_SINGLE_COIL    :
                 case Function::WRITE_SINGLE_REGISTER: mState = STATE_OUT_ADDR_H; break;

                 default: Reset(); return;
                 }
                 break;

             case STATE_OUT_ADDR_H: mAddress = aByte << 8; mState = STATE_OUT_ADDR_L; break;
             case STATE_OUT_ADDR_L: mAddress |= aByte    ; mState = STATE_VALUE_H   ; break;

             case STATE_QTY_H: mQty  = aByte << 8; mState = STATE_QTY_L; break;
             case STATE_QTY_L: mQty |= aByte     ; mState = STATE_CRC_L; break;

             case STATE_START_ADDR_H: mAddress  = aByte << 8; mState = STATE_START_ADDR_L; break;
             case STATE_START_ADDR_L: mAddress |= aByte     ; mState = STATE_QTY_H       ; break;

             case STATE_VALUE_H: mBuffer[0] = aByte; mState = STATE_VALUE_L; break;
             case STATE_VALUE_L: mBuffer[1] = aByte; mState = STATE_CRC_L  ; break;

             default: assert(false);
             }

             mCRC.Compute(aByte);
        }

        void Slave_Serial::Reset()
        {
            mSilentCounter = 0;
            mState         = STATE_DEV_ADDR;

            mCRC.Reset();
        }

        // ===== Slave ======================================================

        void Slave_Serial::SendException(Function aFunction, Exception aException)
        {
            uint8_t lBuffer[5];

            WriteUInt8(lBuffer, 0, mDeviceAddress);
            WriteUInt8(lBuffer, 1, static_cast<uint8_t>(aFunction));
            WriteUInt8(mBuffer, 2, static_cast<uint8_t>(aException));

            CRC::Add(lBuffer, sizeof(lBuffer));

            SendData(lBuffer, sizeof(lBuffer));

            Reset();
        }

        void Slave_Serial::SendResponse_A(Function aFunction, uint8_t aSize_byte, const void* aData)
        {
            assert(NULL != aData);

            uint8_t lBuffer[260];

            WriteUInt8(lBuffer, 0, GetDeviceAddress());
            WriteUInt8(lBuffer, 1, static_cast<uint8_t>(aFunction));
            WriteUInt8(lBuffer, 2, aSize_byte);

            memcpy(lBuffer + 3, aData, aSize_byte);

            unsigned int lSize_byte = 3 + aSize_byte + 2;

            CRC::Add(lBuffer, lSize_byte);

            SendData(lBuffer, lSize_byte);

            Reset();
        }

        void Slave_Serial::SendResponse_B(Function aFunction, Address aAddress, const void* aData)
        {
            assert(NULL != aData);

            uint8_t lBuffer[8];

            WriteUInt8(lBuffer, 0, GetDeviceAddress());
            WriteUInt8(lBuffer, 1, static_cast<uint8_t>(aFunction));

            WriteUInt16(lBuffer, 2, aAddress);

            memcpy(lBuffer + 4, aData, sizeof(RegisterValue));

            CRC::Add(lBuffer, sizeof(lBuffer));

            SendData(lBuffer, sizeof(lBuffer));

            Reset();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Slave_Serial::OnRequest()
        {
            if (mReceivedCRC == mCRC)
            {
                switch (mFunction)
                {
                case Function::READ_COILS          : OnRequest_A_Bit(mFunction, mAddress, mQty, &mOnReadCoils); break;
                case Function::READ_DISCRETE_INPUTS: OnRequest_A_Bit(mFunction, mAddress, mQty, &mOnReadDiscreteInputs); break;

                case Function::READ_HOLDING_REGISTERS: OnRequest_A_Word(mFunction, mAddress, mQty, &mOnReadHoldingRegisters); break;
                case Function::READ_INPUT_REGISTERS  : OnRequest_A_Word(mFunction, mAddress, mQty, &mOnReadInputRegisters); break;

                case Function::WRITE_SINGLE_COIL    : OnRequest_B(mFunction, mAddress, mBuffer, &mOnWriteSingleCoil); break;
                case Function::WRITE_SINGLE_REGISTER: OnRequest_B(mFunction, mAddress, mBuffer, &mOnWriteSingleRegister); break;

                default: SendException(mFunction, Exception::ILLEGAL_FUNCTION);
                }
            }
            else
            {
                Reset();
            }
        }

    }
}
