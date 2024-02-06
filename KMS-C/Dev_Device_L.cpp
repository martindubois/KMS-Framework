
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device_L.cpp

#include "Component.h"

// ===== System =============================================================
#include <sys/ioctl.h>

// ===== Includes ===========================================================
#include <KMS/Dev/Device.h>

KMS_RESULT_STATIC(RESULT_CONTROL_FAILED);

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // ===== IDevice ====================================================

        bool Device::Connect(unsigned int aFlags)
        {
            // TODO
            return false;
        }

        void Device::Disconnect()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            auto lRet = close(mHandle);
            assert(0 == lRet);

            mHandle = INVALID_HANDLE_VALUE;
        }

        unsigned int Device::Control(unsigned int aCode, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            uint8_t* lBuffer = nullptr;
            void   * lInOut;

            if (aInSize_byte <= aOutSize_byte)
            {
                lInOut = aOut;

                if (0 < aInSize_byte)
                {
                    assert(nullptr != aIn);
                    assert(nullptr != aOut);

                    memcpy(lInOut, aIn, aInSize_byte);
                }
            }
            else if (0 == aOutSize_byte)
            {
                lInOut = const_cast<void*>(aIn);
            }
            else
            {
                assert(nullptr != aIn);
                assert(nullptr != aOut);

                lBuffer = new uint8_t [aInSize_byte];

                lInOut = lBuffer;

                memcpy(lInOut, aIn, aInSize_byte);
            }

            auto lResult_byte = ioctl(mHandle, aCode, lInOut);
            KMS_EXCEPTION_ASSERT(0 <= lResult_byte, RESULT_CONTROL_FAILED, "ioctl failed", lResult_byte);

            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte, RESULT_CONTROL_FAILED, "Invalid control result", lResult_byte);
            if ((0 < lResult_byte) && (nullptr != lBuffer))
            {
                memcpy(aOut, lInOut, lResult_byte);
            }

            if (NULL != lBuffer)
            {
                delete [] lBuffer;
            }

            return lResult_byte;
        }

        unsigned int Device::Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags)
        {
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);
            assert(0 == (aFlags & ~FLAG_READ_ALL));

            assert(INVALID_HANDLE_VALUE != mHandle);

            auto lResult_byte = read(mHandle, aOut, aOutSize_byte);
            KMS_EXCEPTION_ASSERT(0 <= lResult_byte, RESULT_READ_FAILED, "Cannot read from the device", lResult_byte);

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

            auto lRet_byte = write(mHandle, aIn, aInSize_byte);
            KMS_EXCEPTION_ASSERT(0 <= lRet_byte, RESULT_WRITE_FAILED, "Cannot write to the device", lRet_byte);

            KMS_EXCEPTION_ASSERT(aInSize_byte == lRet_byte, RESULT_WRITE_FAILED, "The device did not acceps the expected amount of data", lRet_byte);
            return true;
        }

        Device::operator int ()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            return mHandle;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // Private
        // //////////////////////////////////////////////////////////////////

        void Device::Construct_OSDep() {}

    }
}
