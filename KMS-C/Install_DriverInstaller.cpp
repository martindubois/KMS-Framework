
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Install_DriverInstaller.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Install/DriverInstaller.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_UNINSTALL_DRIVER("UninstallDriver = false | true");

namespace KMS
{
    namespace Install
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const bool DriverInstaller::UNINSTALL_DRIVER_DEFAULT = false;

        DriverInstaller::DriverInstaller()
            : mUninstallDriver(UNINSTALL_DRIVER_DEFAULT)
        {
            AddEntry("UninstallDriver", &mUninstallDriver, false, &MD_UNINSTALL_DRIVER);

            Construct_OSDep();
        }

        void DriverInstaller::Run()
        {
            if (mUninstallDriver)
            {
                UninstallDriver();
            }
            else
            {
                Run_OSDep();
            }
        }

    }
}
