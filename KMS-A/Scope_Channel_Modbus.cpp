
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Scope_Channel_Modbus.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Scope/Channel_Modbus.h>

KMS_RESULT_STATIC(RESULT_MODBUS_ERROR);

namespace KMS
{
    namespace Scope
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Channel_Modbus::Init(Modbus::Master* aMaster, Modbus::Address aAddress)
        {
            assert(nullptr != aMaster);

            assert(nullptr == mMaster);

            mAddress = aAddress;
            mMaster = aMaster;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Channel ====================================================

        void Channel_Modbus_Coil::Sample(unsigned int* aY_px, bool* aTrig)
        {
            auto lMaster = GetMaster();
            assert(nullptr != lMaster);

            bool lValue;

            auto lRet = lMaster->ReadCoil(GetAddress(), &lValue);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_MODBUS_ERROR, "ReadCoil failed", "");

            SetSample(lValue);

            Channel_Modbus::Sample(aY_px, aTrig);
        }

        void Channel_Modbus_DiscreteInput::Sample(unsigned int* aY_px, bool* aTrig)
        {
            auto lMaster = GetMaster();
            assert(nullptr != lMaster);

            bool lValue;

            auto lRet = lMaster->ReadDiscreteInput (GetAddress(), &lValue);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_MODBUS_ERROR, "ReadCoil failed", "");

            SetSample(lValue);

            Channel_Modbus::Sample(aY_px, aTrig);
        }

        void Channel_Modbus_HoldingRegister::Sample(unsigned int* aY_px, bool* aTrig)
        {
            auto lMaster = GetMaster();
            assert(nullptr != lMaster);

            Modbus::RegisterValue lValue;

            auto lRet = lMaster->ReadHoldingRegister(GetAddress(), &lValue);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_MODBUS_ERROR, "ReadCoil failed", "");

            SetSample(lValue);

            Channel_Modbus::Sample(aY_px, aTrig);
        }

        void Channel_Modbus_InputRegister::Sample(unsigned int* aY_px, bool* aTrig)
        {
            auto lMaster = GetMaster();
            assert(nullptr != lMaster);

            Modbus::RegisterValue lValue;

            auto lRet = lMaster->ReadInputRegister(GetAddress(), &lValue);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_MODBUS_ERROR, "ReadCoil failed", "");

            SetSample(lValue);

            Channel_Modbus::Sample(aY_px, aTrig);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Channel_Modbus::Channel_Modbus() : mAddress(0), mMaster(nullptr) {}

        Modbus::Address Channel_Modbus::GetAddress() const { return mAddress; }

        Modbus::Master* Channel_Modbus::GetMaster()
        {
            assert(nullptr != mMaster);

            return mMaster;
        }

    }
}
