
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/SetupDi.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Local ==============================================================
#include "SetupDi.h"

#pragma comment (lib, "setupapi.lib")

KMS_RESULT_STATIC(RESULT_SETUP_API_ERROR);

namespace SetupDi
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CallClassInstaller(DI_FUNCTION aFunction, HDEVINFO aDevInfo, SP_DEVINFO_DATA* aDevInfoData)
    {
        assert(INVALID_HANDLE_VALUE != aDevInfo);
        assert(nullptr != aDevInfoData);

        unsigned int lSize_byte = sizeof(SP_DEVINFO_DATA);

        memset(aDevInfoData, 0, lSize_byte);

        aDevInfoData->cbSize = lSize_byte;

        auto lRet = SetupDiCallClassInstaller(aFunction, aDevInfo, aDevInfoData);
        KMS_EXCEPTION_ASSERT(lRet, RESULT_SETUP_API_ERROR, "SetupDiCallClassInstaller failed", aFunction);
    }

    void DestroyDeviceInfoList(HDEVINFO aDevInfo)
    {
        assert(INVALID_HANDLE_VALUE != aDevInfo);

        auto lRet = SetupDiDestroyDeviceInfoList(aDevInfo);
        KMS_EXCEPTION_ASSERT(lRet, RESULT_SETUP_API_ERROR, "SetupDiDestroyDeviceInfoList failed", "");
    }

    extern bool EnumDeviceInfo(HDEVINFO aDevInfo, unsigned int aIndex, SP_DEVINFO_DATA* aDevInfoData)
    {
        assert(INVALID_HANDLE_VALUE != aDevInfo);
        assert(nullptr != aDevInfoData);

        unsigned int lSize_byte = sizeof(SP_DEVINFO_DATA);

        memset(aDevInfoData, 0, lSize_byte);

        aDevInfoData->cbSize = lSize_byte;

        return SetupDiEnumDeviceInfo(aDevInfo, aIndex, aDevInfoData);
    }

    extern bool EnumDeviceInterfaces(HDEVINFO aDevInfo, SP_DEVINFO_DATA* aDevInfoData, const ::GUID& aInterface, SP_DEVICE_INTERFACE_DATA* aDevIntData)
    {
        assert(INVALID_HANDLE_VALUE != aDevInfo);
        assert(nullptr != aDevInfoData);
        assert(nullptr != aDevIntData);

        unsigned int lSize_byte = sizeof(SP_DEVICE_INTERFACE_DATA);

        memset(aDevIntData, 0, lSize_byte);

        aDevIntData->cbSize = lSize_byte;

        return (TRUE == SetupDiEnumDeviceInterfaces(aDevInfo, aDevInfoData, &aInterface, 0, aDevIntData));
    }

    extern HDEVINFO GetClassDevs_Interface(const ::GUID& aInterface)
    {
        auto lResult = SetupDiGetClassDevs(&aInterface, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
        KMS_EXCEPTION_ASSERT(INVALID_HANDLE_VALUE != lResult, RESULT_SETUP_API_ERROR, "SetupDiGetClassDevs failed", "");

        return lResult;
    }

    extern HDEVINFO GetClassDevs_Class(const ::GUID& aClass)
    {
        auto lResult = SetupDiGetClassDevsEx(&aClass, nullptr, nullptr, DIGCF_PRESENT, nullptr, nullptr, nullptr);
        KMS_EXCEPTION_ASSERT(INVALID_HANDLE_VALUE != lResult, RESULT_SETUP_API_ERROR, "SetupDiGetClassDevsEx failed", "");

        return lResult;
    }

    extern void GetDeviceInfoListDetail(HDEVINFO aDevInfo, SP_DEVINFO_LIST_DETAIL_DATA* aDetail)
    {

    }

    extern void GetDeviceInterfaceDetail(HDEVINFO aDevInfo, SP_DEVICE_INTERFACE_DATA* aDevIntData, SP_DEVICE_INTERFACE_DETAIL_DATA* aDetail, unsigned int aSize_byte)
    {
        assert(INVALID_HANDLE_VALUE != aDevInfo);
        assert(nullptr != aDevIntData);
        assert(nullptr != aDetail);
        assert(sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) < aSize_byte);

        memset(aDetail, 0, aSize_byte);

        aDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        DWORD lInfo_byte;

        auto lRet = SetupDiGetDeviceInterfaceDetail(aDevInfo, aDevIntData, aDetail, aSize_byte, &lInfo_byte, NULL);
        KMS_EXCEPTION_ASSERT(lRet, RESULT_SETUP_API_ERROR, "SetupDiGetDeviceInterfaceDetail failed", "");

        assert(sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) <= lInfo_byte);
        assert(aSize_byte >= lInfo_byte);
    }

    extern void SetClassInstallParams(HDEVINFO aDevInfo, SP_DEVINFO_DATA* aDevInfoData, SP_CLASSINSTALL_HEADER* aHeader, unsigned int aSize_byte)
    {

    }

    extern void CM_GetDevice_ID_Ex(DEVINST aDevInst, void* aOut, unsigned int aOutSize_byte)
    {

    }

}

