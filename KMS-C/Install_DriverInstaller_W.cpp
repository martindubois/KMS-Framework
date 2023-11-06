
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Install_DriverInstaller_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Install/DriverInstaller.h>

#include "SetupDi.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_DELETE_DEVICE_CLASS("DeleteDeviceClass = false | true");
static const KMS::Cfg::MetaData MD_DEVICE_CLASS       ("DeviceClass = {GUID}");

namespace KMS
{
    namespace Install
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const ::GUID DriverInstaller::DEVICE_CLASS_DEFAULT;
        const bool   DriverInstaller::DELETE_DEVICE_CLASS_DEFAULT = false;

        void DriverInstaller::DeleteDeviceClass()
        {
            auto lDevInfo = SetupDi::GetClassDevs_Class(mDeviceClass);

            try
            {
                DWORD lIndex = 0;

                SP_DEVINFO_DATA lDevInfoData;

                while (SetupDi::EnumDeviceInfo(lDevInfo, lIndex, &lDevInfoData))
                {
                    char lId[PATH_LENGTH];

                    SetupDi::CM_GetDevice_ID_Ex(lDevInfoData.DevInst, lId, sizeof(lId));

                    SP_REMOVEDEVICE_PARAMS lParams;

                    lParams.ClassInstallHeader.cbSize = sizeof(lParams);
                    lParams.ClassInstallHeader.InstallFunction = DIF_REMOVE;
                    lParams.Scope = DI_REMOVEDEVICE_GLOBAL;
                    lParams.HwProfile = 0;

                    SetupDi::SetClassInstallParams(lDevInfo, &lDevInfoData, &lParams.ClassInstallHeader, sizeof(lParams));

                    SetupDi::CallClassInstaller(DIF_REMOVE, lDevInfo, &lDevInfoData);

                    lIndex++;
                }
            }
            catch (...)
            {
                SetupDi::DestroyDeviceInfoList(lDevInfo);

                throw;
            }

            SetupDi::DestroyDeviceInfoList(lDevInfo);
        }

        void DriverInstaller::UninstallDriver()
        {
            DeleteDeviceClass();
        }

        // ===== DI::Dictionary =============================================
        void DriverInstaller::Validate()
        {
            DI::Dictionary::Validate();

            if (mDeleteDeviceClass || mUninstallDriver)
            {
                KMS_EXCEPTION_ASSERT(mDeviceClass.IsValid(), RESULT_INVALID_FORMAT, "A device class must be configured", "");
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void DriverInstaller::Construct_OSDep()
        {
            AddEntry("DeleteDeviceClass", &mDeleteDeviceClass, false, &MD_DELETE_DEVICE_CLASS);
            AddEntry("DeviceClass"      , &mDeviceClass      , false, &MD_DEVICE_CLASS);
        }

        void DriverInstaller::Run_OSDep()
        {
            if (mDeleteDeviceClass)
            {
                DeleteDeviceClass();
            }
        }

    }
}
