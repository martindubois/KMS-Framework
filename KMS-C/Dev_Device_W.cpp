
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
        void Device::SetInterface  (const GUID& aI) { mInterface = aI; mInterface_Valid = true; }
        void Device::SetLocation   (const char* aL ) { assert(NULL != aL);  mLocation = aL; }

        void Device::Connect(unsigned int aFlags)
        {
            if (IsConnected())
            {
                Disconnect();
            }

            assert(INVALID_HANDLE_VALUE == mHandle);

            if (0 == mLink.size())
            {
                if (0 < mHardwareId.size())
                {
                    if (0 < mLocation.size())
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

            mHandle = CreateFile(mLink.c_str(), lAccess, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (INVALID_HANDLE_VALUE == mHandle)
            {
                KMS_EXCEPTION_WITH_INFO(COM_CONNECT, "CreateFile failed", mLink.c_str());
            }
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
                KMS_EXCEPTION(DEVICE, "ReadFile failed");
            }

            if (0 != (aFlags && FLAG_READ_ALL))
            {
                if (aOutSize_byte != lResult_byte)
                {
                    KMS_EXCEPTION_WITH_INFO(DEVICE, "The device did not return the expected amount of data", lResult_byte);
                }
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
                KMS_EXCEPTION(DEVICE, "WriteFile failed");
            }

            if (aInSize_byte != lInfo_byte)
            {
                KMS_EXCEPTION_WITH_INFO(DEVICE, "The device did not acceps the expected amount of data", lInfo_byte);
            }
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

        // Private
        // //////////////////////////////////////////////////////////////////

        GUID Device::ToGUID(const char* aIn)
        {
            assert(NULL != aIn);

            GUID lResult;

            memset(&lResult, 0, sizeof(lResult));

            // {00000000-0000-0000-0000-000000000000}
            // 01234567890123456789012345678901234567

            uint32_t lValue = 0;

            for (unsigned int i = 1; i <= 36; i++)
            {
                lValue <<= 8;

                switch (aIn[i])
                {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': lValue |= aIn[i] - '0'; break;

                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f': lValue |= aIn[i] - 'a' + 10; break;

                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F': lValue |= aIn[i] - 'A' + 10; break;

                case '-': break;

                default: KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid GUID format", aIn);
                }

                switch (i)
                {
                case  8: lResult.Data1 = lValue; lValue = 0; break;
                case 13: lResult.Data2 = lValue; lValue = 0; break;
                case 18: lResult.Data3 = lValue; lValue = 0; break;

                case 21:
                case 23: lResult.Data4[(i - 21) / 2] = lValue; lValue = 0; break;

                case 26:
                case 28:
                case 30:
                case 32:
                case 34:
                case 36: lResult.Data4[2 + (i - 26) / 2] = lValue; lValue = 0; break;
                }
            }

            return lResult;
        }

    }
}
