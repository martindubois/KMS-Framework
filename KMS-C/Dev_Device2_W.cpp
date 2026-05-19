
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device2_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dev/Device2.h>

// ===== Local ==============================================================
#include "SetupDi.h"

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        unsigned int Device2::GetCount(const Device2_Config& aConfig)
        {
            unsigned int lResult = 0;

            auto lDevInfo = SetupDi::GetClassDevs_Interface(aConfig.mInterface);
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

                if (SetupDi::EnumDeviceInterfaces(lDevInfo, &lDevInfoData, aConfig.mInterface, &lDevIntData))
                {
                    lResult++;
                }
            }

            return lResult;
        }

        bool Device2_Config::IsInterfaceValid() const
        {
            auto lBytes = reinterpret_cast<const uint8_t*>(&mInterface);

            for (unsigned int i = 0; i < sizeof(mInterface); i++)
            {
                if (0 != lBytes[i])
                {
                    return true;
                }
            }

            return false;
        }

        bool Device2_Config::IsValid(char* aMsg, unsigned int aMsgSize_byte) const
        {
            if ((nullptr != aMsg) && (0 < aMsgSize_byte))
            {
                memset(aMsg, 0, aMsgSize_byte);
            }

            if ((!IsInterfaceValid()) && (!(IsLinkValid())))
            {
                if ((nullptr != aMsg) && (0 < aMsgSize_byte))
                {
                    strncpy_s(aMsg SizeInfoV(aMsgSize_byte), "The interface and the link are not valid", aMsgSize_byte - 1);
                }

                return false;
            }

            return true;
        }

        // NOT TESTED
        void Device2_Config::LinkFromInterfaceAndIndex()
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

                    // NOT TESTED
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

                        SetLink(lDetail->DevicePath);

                        break;
                    }
                }
            }
            catch (...)
            {
                SetupDi::DestroyDeviceInfoList(lDevInfo);

                throw;
            }

            // NOT TESTED
            SetupDi::DestroyDeviceInfoList(lDevInfo);
        }

        // ===== IDevice ====================================================

        bool Device2::Connect()
        {
            if (IsConnected())
            {
                Disconnect();
            }

            assert(INVALID_HANDLE_VALUE == mHandle);

            if (!mConfig.IsLinkValid())
            {
                KMS_EXCEPTION_ASSERT(mConfig.IsInterfaceValid(), RESULT_INVALID_CONFIG, "Invalid device interface", "");

                // NOT TESTED
                mConfig.LinkFromInterfaceAndIndex();
            }

            DWORD lAccess = 0;
            DWORD lFlags  = 0;

            if (0 != (mConnectFlags & FLAG_ACCESS_READ )) { lAccess |= GENERIC_READ; }
            if (0 != (mConnectFlags & FLAG_ACCESS_WRITE)) { lAccess |= GENERIC_WRITE; }
            if (0 != (mConnectFlags & FLAG_OVERLAPPED  )) { lFlags |= FILE_FLAG_OVERLAPPED; }

            mHandle = CreateFile(mConfig.mLink, lAccess, 0, NULL, OPEN_EXISTING, lFlags, NULL);
            KMS_EXCEPTION_ASSERT(INVALID_HANDLE_VALUE != mHandle, RESULT_CONNECT_FAILED, "CreateFile failed", mConfig.mLink);

            return true;
        }

        unsigned int Device2::Control(unsigned int aCode, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lResult_byte;

            auto lRet = DeviceIoControl(mHandle, aCode, const_cast<void*>(aIn), aInSize_byte, aOut, aOutSize_byte, &lResult_byte, NULL);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_CONTROL_FAILED, "DeviceIoControl failed", aCode);

            // NOT TESTED
            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte, RESULT_CONTROL_FAILED, "Invalid control result (NOT TESTED)", lResult_byte);

            return lResult_byte;
        }

        // ===== Stream::IStream ============================================

        void Device2::Disconnect()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            auto lRet = CloseHandle(mHandle);
            assert(lRet);

            mHandle = INVALID_HANDLE_VALUE;
        }

        unsigned int Device2::Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags)
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

        bool Device2::Write(const void* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != aIn);
            assert(0 < aInSize_byte);

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lInfo_byte;

            auto lRet = WriteFile(mHandle, aIn, aInSize_byte, &lInfo_byte, NULL);
            KMS_EXCEPTION_ASSERT(lRet, RESULT_WRITE_FAILED, "Cannot write to the device (NOT TESTED)", aInSize_byte);

            KMS_EXCEPTION_ASSERT(aInSize_byte == lInfo_byte, RESULT_WRITE_FAILED, "The device did not acceps the expected amount of data (NOT TESTED)", lInfo_byte);
            return true;
        }

        Device2::operator HANDLE ()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            return mHandle;
        }

    }
}
