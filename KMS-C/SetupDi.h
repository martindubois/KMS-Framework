
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/SetupDi.h

#pragma once

// ===== Windows ============================================================
#include <cfgmgr32.h>
#include <setupapi.h>

namespace SetupDi
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    extern void CallClassInstaller(DI_FUNCTION aFunction, HDEVINFO aDevInfo, SP_DEVINFO_DATA* aDevInfoData);

    extern void DestroyDeviceInfoList(HDEVINFO aDevInfo);

    extern bool EnumDeviceInfo(HDEVINFO aDevInfo, unsigned int aIndex, SP_DEVINFO_DATA *aDevInfoData);

    extern bool EnumDeviceInterfaces(HDEVINFO aDevInfo, SP_DEVINFO_DATA* aDevInfoData, const ::GUID& aInterface, SP_DEVICE_INTERFACE_DATA* aDevIntData);

    extern HDEVINFO GetClassDevs_Interface(const ::GUID& aInterface);

    extern HDEVINFO GetClassDevs_Class(const ::GUID& aClass);

    extern void GetDeviceInfoListDetail(HDEVINFO aDevInfo, SP_DEVINFO_LIST_DETAIL_DATA* aDetail);
    
    extern void GetDeviceInterfaceDetail(HDEVINFO aDevInfo, SP_DEVICE_INTERFACE_DATA* aDevIntData, SP_DEVICE_INTERFACE_DETAIL_DATA* aDetail, unsigned int aSize_byte);

    extern void SetClassInstallParams(HDEVINFO aDevInfo, SP_DEVINFO_DATA* aDevInfoData, SP_CLASSINSTALL_HEADER* aHeader, unsigned int aSize_byte);

    extern void CM_GetDevice_ID_Ex(DEVINST aDevInst, void* aOut, unsigned int aOutSize_byte);

}
