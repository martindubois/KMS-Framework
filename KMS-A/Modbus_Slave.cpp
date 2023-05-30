
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Slave.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Modbus/Slave.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_DEVICE_ADDRESS (1)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_DEVICE_ADDRESS("DeviceAddress = {Address}");

#define MSG_ITERATE (1)

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Slave::SetDeviceAddress(DeviceAddress aDA) { mDeviceAddress = aDA; }

        void Slave::Run()
        {
            while (!mStopped)
            {
                try
                {
                    if (!KMS_MSG_SUCCESS(OnIterate()))
                    {
                        break;
                    }
                }
                KMS_CATCH
            }
        }

        void Slave::Stop() { mStopped = true; }

        // ===== Msg::IReceiver =============================================

        unsigned int Slave::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult;

            if (MSG_ITERATE == aCode) { lResult = OnIterate(); }
            else
            {
                assert(false);
                lResult = Msg::IReceiver::MSG_IGNORED;
            }

            return lResult;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Slave::Slave() : mStopped(false)
        {
            AddEntry("DeviceAddress", &mDeviceAddress, false, &MD_DEVICE_ADDRESS);
        }

        DeviceAddress Slave::GetDeviceAddress() const { return mDeviceAddress; }

        void Slave::OnRequest_A_Bit(Function aFunction, uint16_t aStartAddr, uint16_t aQty, Msg::Destination* aDst)
        {
            assert(NULL != aDst);

            MsgData lData;

            lData.mException = Exception::NO_EXCEPTION;

            uint8_t lBuffer[256];

            memset(&lBuffer, 0, sizeof(lBuffer));

            lData.mBuffer    = lBuffer;
            lData.mQty       = aQty;
            lData.mStartAddr = aStartAddr;

            auto lRet = aDst->Send(this, &lData);
            if (KMS_MSG_SUCCESS(lRet))
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

            if (Exception::NO_EXCEPTION != lData.mException)
            {
                SendException(aFunction, lData.mException);
            }
        }

        void Slave::OnRequest_A_Word(Function aFunction, uint16_t aStartAddr, uint16_t aQty, Msg::Destination* aDst)
        {
            assert(NULL != aDst);

            MsgData lData;

            lData.mException = Exception::NO_EXCEPTION;

            uint8_t lBuffer[256];

            lData.mBuffer    = lBuffer;
            lData.mQty       = aQty;
            lData.mStartAddr = aStartAddr;

            auto lRet = aDst->Send(this, &lData);
            if (KMS_MSG_SUCCESS(lRet))
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

            if (Exception::NO_EXCEPTION != lData.mException)
            {
                SendException(aFunction, lData.mException);
            }
        }

        void Slave::OnRequest_B(Function aFunction, uint16_t aStartAddr, void * aData, Msg::Destination* aDst)
        {
            assert(NULL != aDst);

            MsgData lData;

            lData.mException = Exception::NO_EXCEPTION;

            lData.mBuffer    = reinterpret_cast<uint8_t*>(aData);
            lData.mQty       = 1;
            lData.mStartAddr = aStartAddr;

            auto lRet = aDst->Send(this, &lData);
            if (KMS_MSG_SUCCESS(lRet))
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

            if (Exception::NO_EXCEPTION != lData.mException)
            {
                SendException(aFunction, lData.mException);
            }
        }

    }
}

