
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Stream/IStream.h

#pragma once

namespace KMS
{
    namespace Stream
    {

        class IStream
        {

        public:

            static const unsigned int ERROR_READ = 0xffffffff;

            static const unsigned int FLAG_READ_ALL = 0x01000000;

            virtual bool IsConnected() const = 0;

            virtual bool Connect() = 0;

            virtual void Disconnect() = 0;

            // aFlags  FLAG_READ_ALL  Some implementation may not support it.
            //
            // Return  ERROR_READ
            //         Other       The size of read data in byte
            //
            // Exception  RESULT_READ_FAILED
            //            RESULT_RECEIVE_FAILED
            virtual unsigned int Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags = 0) = 0;

            // Return  false  Error
            //         true   OK
            //
            // Exception  RESULT_SEND_FAILED
            //            RESULT_WRITE_FAILED
            virtual bool Write(const void* aIn, unsigned int aInSize_byte) = 0;

        };

    }
}