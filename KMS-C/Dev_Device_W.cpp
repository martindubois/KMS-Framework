
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

#include <SetupAPI.h>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Dev/Device.h>

// ===== Local ==============================================================
#include "SetupDi.h"

KMS_RESULT_STATIC(RESULT_CONTROL_FAILED);
KMS_RESULT_STATIC(RESULT_NO_DEVICE);

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_INTERFACE("Interface = {GUID}");

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const ::GUID Device::INTERFACE_DEFAULT;

        unsigned int Device::GetCount(const ::GUID* aInterface)
        {
            unsigned int lResult = 0;

            auto lDevInfo = SetupDi::GetClassDevs_Interface(*aInterface);
            assert(INVALID_HANDLE_VALUE != lDevInfo);

            for (unsigned int i = 0;; i++)
            {
                SP_DEVINFO_DATA lDevInfoData;

                auto lRet = SetupDi::EnumDeviceInfo(lDevInfo, i, &lDevInfoData);
                if (!lRet)
                {
                    break;
                }

                SP_DEVICE_INTERFACE_DATA lDevIntData;

                if (SetupDi::EnumDeviceInterfaces(lDevInfo, &lDevInfoData, *aInterface, &lDevIntData))
                {
                    lResult++;
                }
            }

            return lResult;
        }

        // ===== Stream::IStream ============================================

        bool Device::Connect()
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

            if (0 != (mConnectFlags & FLAG_ACCESS_READ)) { lAccess |= GENERIC_READ; }
            if (0 != (mConnectFlags & FLAG_ACCESS_WRITE)) { lAccess |= GENERIC_WRITE; }
            if (0 != (mConnectFlags & FLAG_OVERLAPPED)) { lFlags |= FILE_FLAG_OVERLAPPED; }

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

        unsigned int Device::Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags)
        {
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);
            assert(0 == (aFlags & ~Stream::IStream::FLAG_READ_ALL));

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lResult_byte;

            auto lRet = ReadFile(mHandle, aOut, aOutSize_byte, &lResult_byte, NULL);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_READ_FAILED, "Cannot read from the device", aOutSize_byte);

            if (0 != (aFlags && Stream::IStream::FLAG_READ_ALL))
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

        // ===== IDevice ====================================================

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
            auto lDevInfo = SetupDi::GetClassDevs_Interface(mInterface);
            assert(INVALID_HANDLE_VALUE != lDevInfo);

            try
            {
                unsigned int lIndex = 0;

                for (unsigned int i = 0;; i++)
                {
                    SP_DEVINFO_DATA lDevInfoData;

                    auto lRet = SetupDi::EnumDeviceInfo(lDevInfo, i, &lDevInfoData);
                    KMS_EXCEPTION_ASSERT(lRet, RESULT_NO_DEVICE, "No device", "");

                    SP_DEVICE_INTERFACE_DATA lDevIntData;

                    if (SetupDi::EnumDeviceInterfaces(lDevInfo, &lDevInfoData, mInterface, &lDevIntData))
                    {
                        if (mIndex > lIndex)
                        {
                            lIndex++;
                            continue;
                        }

                        uint8_t lBuffer[PATH_LENGTH];
                        auto lDetail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA*>(lBuffer);

                        SetupDi::GetDeviceInterfaceDetail(lDevInfo, &lDevIntData, lDetail, sizeof(lBuffer));

                        mLink = lDetail->DevicePath;

                        break;
                    }
                }
            }
            catch (...)
            {
                SetupDi::DestroyDeviceInfoList(lDevInfo);

                throw;
            }

            SetupDi::DestroyDeviceInfoList(lDevInfo);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Device::Construct_OSDep()
        {
            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mInterface, false);  AddEntry("Interface", lEntry, &MD_INTERFACE);
        }

    }
}
