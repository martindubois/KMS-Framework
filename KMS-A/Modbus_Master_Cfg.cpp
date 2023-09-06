
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

#define RETRY_COUNT_MAX (8)

static const KMS::Cfg::MetaData MD_DEVICE_ADDRESS("DeviceAddress = {Address}");
static const KMS::Cfg::MetaData MD_RETRY_COUNT   ("RetryCount = {Count}");

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Master_Cfg::Master_Cfg(Master* aMaster)
            : mDeviceAddress(&aMaster->mDeviceAddress)
            , mRetryCount   (&aMaster->mRetryCount)
        {
            AddEntry("DeviceAddress", &mDeviceAddress, false, &MD_DEVICE_ADDRESS);
            AddEntry("RetryCount"   , &mRetryCount   , false, &MD_RETRY_COUNT);
        }

        // ===== DI::Container ==============================================

        void Master_Cfg::Validate() const
        {
            DI::Dictionary::Validate();

            KMS_EXCEPTION_ASSERT(RETRY_COUNT_MAX >= mRetryCount, RESULT_INVALID_CONFIG, "The retry count is not valid", mRetryCount);
        }

    }
}
