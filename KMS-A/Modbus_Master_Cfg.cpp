
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Master_Cfg.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Modbus/Master.h>

#include <KMS/Modbus/Master_Cfg.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_DEVICE_ADDRESS("DeviceAddress = {Address}");
static const KMS::Cfg::MetaData MD_RETRY_COUNT   ("RetryCount = {Count}");

namespace KMS
{
    namespace Modbus
    {

        // Protected
        // //////////////////////////////////////////////////////////////////

        Master_Cfg::Master_Cfg(Master* aMaster)
            : mDeviceAddress_Wrapper(&aMaster->mDeviceAddress)
            , mRetryCount_Wrapper   (&aMaster->mRetryCount)
        {
            AddEntry("DeviceAddress", &mDeviceAddress_Wrapper, false, &MD_DEVICE_ADDRESS);
            AddEntry("RetryCount"   , &mRetryCount_Wrapper   , false, &MD_RETRY_COUNT);
        }

    }
}
