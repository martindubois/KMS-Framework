
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Socket_Client.h
// Library   KMS-B

#pragma once

// ===== Includes ========================================================
#include <KMS/Net/Socket.h>
#include <KMS/Stream/IStream.h>

namespace KMS
{
    namespace Net
    {

        /// \brief TODO Remove use of DI namespace
        class Socket_Client : public Socket, public Stream::IStream
        {

        public:

            static const Net::Address REMOTE_ADDRESS_DEFAULT;

            DI::NetAddress mRemoteAddress;

            Socket_Client(Type aType);

            // ===== Stream::IStream ========================================
            virtual bool IsConnected() const;
            virtual bool Connect();
            virtual void Disconnect();
            virtual unsigned int Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags = 0);
            virtual bool Write(const void* aIn, unsigned int aInSize_byte);

            // ===== Socket =================================================
            virtual ~Socket_Client();

        protected:

            virtual void Connect_Internal();

            // ===== Socket =================================================
            virtual void VerifyState_CLOSED(State aS);
            virtual void VerifyState_OPEN  (State aS);

        };

    }
}
