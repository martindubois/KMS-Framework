
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Slave.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

#include <KMS/Modbus/Slave.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_DEVICE_ADDRESS (1)

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int Slave::MSG_ITERATE = 1;

        void Slave::SetDeviceAddress(DeviceAddress aDA) { mDeviceAddress = aDA; }

        void Slave::Run()
        {
            while (!mStopped)
            {
                try
                {
                    if (!OnIterate())
                    {
                        break;
                    }
                }
                KMS_CATCH
            }
        }

        void Slave::Stop() { mStopped = true; }

        // ===== Msg::IReceiver =============================================

        bool Slave::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            bool lResult = false;

            if (MSG_ITERATE == aCode) { lResult = OnIterate(); }
            else
            {
                assert(false);
            }

            return lResult;
        }

        // ===== Cfg::Configurable ==========================================

        void Slave::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "===== KMS::Modbus::Link =====\n"
                "DeviceAddress\n"
                "    Set the device address to the default value\n"
                "    Default: %u\n"
                "DeviceAddress = {Addr}\n"
                "    Device address\n",
                DEFAULT_DEVICE_ADDRESS);

            Cfg::Configurable::DisplayHelp(aOut);
        }

        bool Slave::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("DeviceAddress") { SetDeviceAddress(DEFAULT_DEVICE_ADDRESS); return true; }
            }
            else
            {
                CFG_CONVERT("DeviceAddress", SetDeviceAddress, Convert::ToUInt8);
            }

            return Cfg::Configurable::SetAttribute(aA, aV);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Slave::Slave()
            : mDeviceAddress(DEFAULT_DEVICE_ADDRESS)
            , mOnReadCoils           (this)
            , mOnReadDiscreteInputs  (this)
            , mOnReadHoldingRegisters(this)
            , mOnReadInputRegisters  (this)
            , mOnWriteSingleCoil     (this)
            , mOnWriteSingleRegister (this)
            , mStopped(false)
        {
        }

        DeviceAddress Slave::GetDeviceAddress() const { return mDeviceAddress; }

        void Slave::OnRequest_A_Bit(Function aFunction, uint16_t aStartAddr, uint16_t aQty, Msg::Sender* aSender)
        {
            assert(NULL != aSender);

            MsgData lData;

            lData.mException = Exception::NO_EXCEPTION;

            if (aSender->IsSet())
            {
                uint8_t lBuffer[256];

                lData.mBuffer    = lBuffer;
                lData.mQty       = aQty;
                lData.mStartAddr = aStartAddr;

                if (aSender->Send(&lData))
                {
                    if (Exception::NO_EXCEPTION == lData.mException)
                    {
                        uint8_t lSize_byte = lData.mQty / 8;

                        if (0 != (lData.mQty % 8))
                        {
                            lSize_byte++;
                        }

                        SendResponse_A(aFunction, lSize_byte, lBuffer);
                    }
                }
                else
                {
                    lData.mException = Exception::ILLEGAL_FUNCTION;
                }
            }
            else
            {
                lData.mException = Exception::ILLEGAL_FUNCTION;
            }

            if (Exception::NO_EXCEPTION != lData.mException)
            {
                SendException(aFunction, lData.mException);
            }
        }

        void Slave::OnRequest_A_Word(Function aFunction, uint16_t aStartAddr, uint16_t aQty, Msg::Sender* aSender)
        {
            assert(NULL != aSender);

            MsgData lData;

            lData.mException = Exception::NO_EXCEPTION;

            if (aSender->IsSet())
            {
                uint8_t lBuffer[256];

                lData.mBuffer    = lBuffer;
                lData.mQty       = aQty;
                lData.mStartAddr = aStartAddr;

                if (aSender->Send(&lData))
                {
                    if (Exception::NO_EXCEPTION == lData.mException)
                    {
                        SendResponse_A(aFunction, static_cast<uint8_t>(sizeof(RegisterValue) * lData.mQty), lBuffer);
                    }
                }
                else
                {
                    lData.mException = Exception::ILLEGAL_FUNCTION;
                }
            }
            else
            {
                lData.mException = Exception::ILLEGAL_FUNCTION;
            }

            if (Exception::NO_EXCEPTION != lData.mException)
            {
                SendException(aFunction, lData.mException);
            }
        }

        void Slave::OnRequest_B(Function aFunction, uint16_t aStartAddr, void * aData, Msg::Sender* aSender)
        {
            assert(NULL != aSender);

            MsgData lData;

            lData.mException = Exception::NO_EXCEPTION;

            if (aSender->IsSet())
            {
                lData.mBuffer    = reinterpret_cast<uint8_t*>(aData);
                lData.mQty       = 1;
                lData.mStartAddr = aStartAddr;

                if (aSender->Send(&lData))
                {
                    if (Exception::NO_EXCEPTION == lData.mException)
                    {
                        SendResponse_B(aFunction, aStartAddr, aData);
                    }
                }
                else
                {
                    lData.mException = Exception::ILLEGAL_FUNCTION;
                }
            }
            else
            {
                lData.mException = Exception::ILLEGAL_FUNCTION;
            }

            if (Exception::NO_EXCEPTION != lData.mException)
            {
                SendException(aFunction, lData.mException);
            }
        }

    }
}

