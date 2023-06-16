
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Slave_Cfg.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Modbus/Slave_Cfg.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_DEVICE_ADDRESS("DeviceAddress = {Address}");

namespace KMS
{
    namespace Modbus
    {

        // Protected
        // //////////////////////////////////////////////////////////////////

        Slave_Cfg::Slave_Cfg(Slave* aSlave) : mDeviceAddress_Wrapper(&aSlave->mDeviceAddress)
        {
            AddEntry("DeviceAddress", &mDeviceAddress_Wrapper, false, &MD_DEVICE_ADDRESS);
        }

    }
}
