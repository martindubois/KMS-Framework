
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Modbus_Slave_Com.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/Slave_IDevice.h>

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Slave_IDevice::Slave_IDevice(Dev::IDevice* aDevice) : mDevice(aDevice)
        {
            assert(NULL != aDevice);
        }

        // ===== Slave ======================================================

        bool Slave_IDevice::Connect()
        {
            assert(NULL != mDevice);

            bool lResult = mDevice->Connect(Dev::IDevice::FLAG_ACCESS_READ | Dev::IDevice::FLAG_ACCESS_WRITE);
            if (lResult)
            {
                mDevice->Write("a", 1);
            }

            return lResult;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        unsigned int Slave_IDevice::OnIterate()
        {
            assert(NULL != mDevice);

            char lBuffer[260];

            auto lRet_byte = mDevice->Read(lBuffer, sizeof(lBuffer));
            if (0 < lRet_byte)
            {
                mSilentCounter = 0;

                for (unsigned int i = 0; i < lRet_byte; i++)
                {
                    OnByte(lBuffer[i]);
                }
            }
            else
            {
                if (3 <= mSilentCounter)
                {
                    Reset();
                }
                else
                {
                    mSilentCounter++;
                }
            }

            return 0;
        }

        // ===== Slave_Serial ===============================================

        void Slave_IDevice::SendData(const uint8_t* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != mDevice);

            mDevice->Write(aIn, aInSize_byte);
        }

    }
}
