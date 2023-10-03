
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

#include <SetupAPI.h>

// ===== Includes ===========================================================
#include <KMS/Dev/Device.h>

KMS_RESULT_STATIC(RESULT_CONTROL_FAILED);
KMS_RESULT_STATIC(RESULT_SETUP_API_ERROR);

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

static void     DestroyDeviceInfoList(HDEVINFO aDevInfo);
static void     EnumDeviceInfo       (HDEVINFO aDevInfo, unsigned int aIndex, SP_DEVINFO_DATA* aDevInfoData);
static bool     EnumDeviceInterfaces (HDEVINFO aDevInfo, SP_DEVINFO_DATA* aDevInfoData, const ::GUID& aInterface, SP_DEVICE_INTERFACE_DATA* aDevIntData);
static HDEVINFO GetClassDevs(const ::GUID& aInterface);
static void     GetDeviceInterfaceDetail(HDEVINFO aDevInfo, SP_DEVICE_INTERFACE_DATA* aDevIntData, SP_DEVICE_INTERFACE_DETAIL_DATA* aDetail, unsigned int aSize_byte);

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const ::GUID Device::INTERFACE_DEFAULT;

        // ===== IDevice ====================================================

        bool Device::IsConnected() const { return INVALID_HANDLE_VALUE != mHandle; }

        void Device::ClearReadBuffer() { assert(false); }

        bool Device::Connect(unsigned int aFlags)
        {
            if (IsConnected())
            {
                Disconnect();
            }

            assert(INVALID_HANDLE_VALUE == mHandle);

            if (0 == mLink.GetLength())
            {
                // NOT TESTED
                if (mInterface.IsValid())
                {
                    LinkFromInterfaceAndIndex();
                }
                else
                {
                    LinkFromIndex();
                }
            }

            DWORD lAccess = 0;
            DWORD lFlags  = 0;

            if (0 != (aFlags & FLAG_ACCESS_READ )) { lAccess |= GENERIC_READ; }
            if (0 != (aFlags & FLAG_ACCESS_WRITE)) { lAccess |= GENERIC_WRITE; }
            if (0 != (aFlags & FLAG_OVERLAPPED  )) { lFlags |= FILE_FLAG_OVERLAPPED; }

            mHandle = CreateFile(mLink, lAccess, 0, NULL, OPEN_EXISTING, lFlags, NULL);
            KMS_EXCEPTION_ASSERT(INVALID_HANDLE_VALUE != mHandle, RESULT_CONNECT_FAILED, "CreateFile failed", mLink);

            return true;
        }

        void Device::Disconnect()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            auto lRet = CloseHandle(mHandle);
            assert(lRet);

            mHandle = INVALID_HANDLE_VALUE;
        }

        unsigned int Device::Control(unsigned int aCode, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lResult_byte;

            auto lRet = DeviceIoControl(mHandle, aCode, const_cast<void*>(aIn), aInSize_byte, aOut, aOutSize_byte, &lResult_byte, NULL);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_CONTROL_FAILED, "DeviceIoControl failed", aCode);

            // NOT TESTED
            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte, RESULT_CONTROL_FAILED, "Invalid control result (NOT TESTED)", lResult_byte);

            return lResult_byte;
        }

        unsigned int Device::Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags)
        {
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);
            assert(0 == (aFlags & ~FLAG_READ_ALL));

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lResult_byte;

            auto lRet = ReadFile(mHandle, aOut, aOutSize_byte, &lResult_byte, NULL);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_READ_FAILED, "Cannot read from the device", aOutSize_byte);

            if (0 != (aFlags && FLAG_READ_ALL))
            {
                KMS_EXCEPTION_ASSERT(aOutSize_byte == lResult_byte, RESULT_READ_FAILED, "The device did not return the expected amount of data", lResult_byte);
            }

            return lResult_byte;
        }

        bool Device::Write(const void* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != aIn);
            assert(0 < aInSize_byte);

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lInfo_byte;

            auto lRet = WriteFile(mHandle, aIn, aInSize_byte, &lInfo_byte, NULL);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_WRITE_FAILED, "Cannot write to the device", aInSize_byte);

            KMS_EXCEPTION_ASSERT(aInSize_byte == lInfo_byte, RESULT_WRITE_FAILED, "The device did not acceps the expected amount of data", lInfo_byte);
            return true;
        }

        Device::operator HANDLE ()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            return mHandle;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // NOT TESTED
        void Device::LinkFromInterfaceAndIndex()
        {
            auto lDevInfo = GetClassDevs(mInterface);
            assert(INVALID_HANDLE_VALUE != lDevInfo);

            try
            {
                unsigned int lIndex = 0;

                for (unsigned int i = 0;; i++)
                {
                    SP_DEVINFO_DATA lDevInfoData;

                    EnumDeviceInfo(lDevInfo, i, &lDevInfoData);

                    SP_DEVICE_INTERFACE_DATA lDevIntData;

                    if (EnumDeviceInterfaces(lDevInfo, &lDevInfoData, mInterface, &lDevIntData))
                    {
                        if (mIndex > lIndex)
                        {
                            lIndex++;
                            continue;
                        }

                        uint8_t lBuffer[PATH_LENGTH];
                        auto lDetail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA*>(lBuffer);

                        GetDeviceInterfaceDetail(lDevInfo, &lDevIntData, lDetail, sizeof(lBuffer));

                        mLink = lDetail->DevicePath;

                        break;
                    }
                }
            }
            catch (...)
            {
                SetupDiDestroyDeviceInfoList(lDevInfo);

                throw;
            }

            DestroyDeviceInfoList(lDevInfo);
        }

    }
}

// Static fonctions
// //////////////////////////////////////////////////////////////////////////

// NOT TESTED

void DestroyDeviceInfoList(HDEVINFO aDevInfo)
{
    assert(INVALID_HANDLE_VALUE != aDevInfo);

    auto lRet = SetupDiDestroyDeviceInfoList(aDevInfo);
    KMS_EXCEPTION_ASSERT(lRet, RESULT_SETUP_API_ERROR, "SetupDiDestroyDeviceInfoList failed", "");
}

void EnumDeviceInfo(HDEVINFO aDevInfo, unsigned int aIndex, SP_DEVINFO_DATA* aDevInfoData)
{
    assert(INVALID_HANDLE_VALUE != aDevInfo);
    assert(nullptr != aDevInfoData);

    unsigned int lSize_byte = sizeof(SP_DEVINFO_DATA);

    memset(aDevInfoData, 0, lSize_byte);

    aDevInfoData->cbSize = lSize_byte;

    auto lRet = SetupDiEnumDeviceInfo(aDevInfo, aIndex, aDevInfoData);
    KMS_EXCEPTION_ASSERT(lRet, RESULT_SETUP_API_ERROR, "SetupDiEnumDeviceInfo failed", aIndex);
}

bool EnumDeviceInterfaces(HDEVINFO aDevInfo, SP_DEVINFO_DATA* aDevInfoData, const ::GUID& aInterface, SP_DEVICE_INTERFACE_DATA* aDevIntData)
{
    assert(INVALID_HANDLE_VALUE != aDevInfo);
    assert(nullptr != aDevInfoData);
    assert(nullptr != aDevIntData);

    unsigned int lSize_byte = sizeof(SP_DEVICE_INTERFACE_DATA);

    memset(aDevIntData, 0, lSize_byte);

    aDevIntData->cbSize = lSize_byte;

    return (TRUE == SetupDiEnumDeviceInterfaces(aDevInfo, aDevInfoData, &aInterface, 0, aDevIntData));
}

HDEVINFO GetClassDevs(const ::GUID& aInterface)
{
    auto lResult = SetupDiGetClassDevs(&aInterface, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
    KMS_EXCEPTION_ASSERT(INVALID_HANDLE_VALUE != lResult, RESULT_SETUP_API_ERROR, "SetupDiGetClassDevs failed", "");

    return lResult;
}

void GetDeviceInterfaceDetail(HDEVINFO aDevInfo, SP_DEVICE_INTERFACE_DATA* aDevIntData, SP_DEVICE_INTERFACE_DETAIL_DATA* aDetail, unsigned int aSize_byte)
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
