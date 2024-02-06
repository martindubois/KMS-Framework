
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dev/Device.h

#pragma once

namespace KMS
{
    namespace Dev
    {

        class IDevice
        {

        public:

            static const unsigned int ERROR_READ = 0xffffffff;

            static const unsigned int FLAG_ACCESS_READ  = 0x00000001;
            static const unsigned int FLAG_ACCESS_WRITE = 0x00000002;
            static const unsigned int FLAG_OVERLAPPED   = 0x00000004;
            static const unsigned int FLAG_READ_ALL     = 0x00000008;

            virtual bool IsConnected() const = 0;

            virtual void ClearReadBuffer() = 0;

            // aFlags FLAG_ACCESS_READ, FLAG_ACCESS_WRITE, FLAG_OVERLAPPED
            virtual bool Connect(unsigned int aFlags) = 0;

            virtual void Disconnect() = 0;

            virtual unsigned int Control(unsigned int aCode, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte) = 0;

            // aFlags FLAG_READ_ALL
            //
            // Return ERROR_READ
            //        Other      The size of read data in byte
            virtual unsigned int Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags = 0) = 0;

            virtual bool Write(const void* aIn, unsigned int aInSize_byte) = 0;

            #ifdef _KMS_LINUX_

                virtual operator int () = 0;

            #endif

            #ifdef _KMS_WINDOWS_

                virtual operator HANDLE () = 0;

            #endif

        };

    }
}
