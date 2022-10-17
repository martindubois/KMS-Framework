
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dev/Device.h>

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Device::operator HANDLE ()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            return mHandle;
        }

        bool Device::IsConnected() const { return INVALID_HANDLE_VALUE != mHandle; }

        void Device::SetHardwareId (const char* aHI) { assert(NULL != aHI); mHardwareId = aHI; }
        void Device::ResetInterface() { mInterface_Valid = false; }
        void Device::SetInterface  (const GUID& aI) { mInterface.Set(aI); mInterface_Valid = true; }
        void Device::SetLocation   (const char* aL) { assert(NULL != aL);  mLocation = aL; }

        void Device::Connect(unsigned int aFlags)
        {
            if (IsConnected())
            {
                Disconnect();
            }

            assert(INVALID_HANDLE_VALUE == mHandle);

            if (0 == mLink.GetLength())
            {
                if (0 < mHardwareId.GetLength())
                {
                    if (0 < mLocation.GetLength())
                    {
                        LinkFromHardwareIdLocationAndIndex();
                    }
                    else
                    {
                        LinkFromHardwareIdAndIndex();
                    }
                }
                else if (mInterface_Valid)
                {
                    LinkFromInterfaceAndIndex();
                }
                else
                {
                    LinkFromIndex();
                }
            }

            DWORD lAccess = 0;

            if (0 != (aFlags & FLAG_READ_ACCESS )) { lAccess |= GENERIC_READ; }
            if (0 != (aFlags & FLAG_WRITE_ACCESS)) { lAccess |= GENERIC_WRITE; }

            mHandle = CreateFile(mLink, lAccess, 0, NULL, OPEN_EXISTING, 0, NULL);
            KMS_EXCEPTION_ASSERT(INVALID_HANDLE_VALUE != mHandle, DEV_CONNECT_FAILED, "CreateFile failed", mLink);
        }

        void Device::Disconnect()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            BOOL lRet = CloseHandle(mHandle);
            assert(lRet);

            mHandle = INVALID_HANDLE_VALUE;
        }

        unsigned int Device::Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags)
        {
            assert(NULL != aOut);
            assert(0 < aOutSize_byte);
            assert(0 == (aFlags & ~FLAG_READ_ALL));

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lResult_byte;

            if (!ReadFile(mHandle, aOut, aOutSize_byte, &lResult_byte, NULL))
            {
                KMS_EXCEPTION(DEV_READ_FAILED, "Cannot read from the device", aOutSize_byte);
            }

            if (0 != (aFlags && FLAG_READ_ALL))
            {
                KMS_EXCEPTION_ASSERT(aOutSize_byte == lResult_byte, DEV_READ_FAILED, "The device did not return the expected amount of data", lResult_byte);
            }

            return lResult_byte;
        }

        void Device::Write(const void* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);
            assert(0 < aInSize_byte);

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lInfo_byte;

            if (!WriteFile(mHandle, aIn, aInSize_byte, &lInfo_byte, NULL))
            {
                KMS_EXCEPTION(DEV_WRITE_FAILED, "Cannot write to the device", aInSize_byte);
            }

            KMS_EXCEPTION_ASSERT(aInSize_byte == lInfo_byte, DEV_WRITE_FAILED, "The device did not acceps the expected amount of data", lInfo_byte);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Device::LinkFromHardwareIdAndIndex()
        {
            // TODO
            assert(false);
        }

        void Device::LinkFromHardwareIdLocationAndIndex()
        {
            // TODO
            assert(false);
        }

        void Device::LinkFromInterfaceAndIndex()
        {
            // TODO
            assert(false);
        }

    }
}
