
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Socket_Client.h
// Library   KMS-B

#pragma once

// ===== Includes ========================================================
#include <KMS/Net/Socket.h>

namespace KMS
{
    namespace Net
    {

        class Socket_Client : public Socket
        {

        public:

            Socket_Client(Type aType);

            void Connect();

            void Disconnect();

            // ===== Configurable attributes ================================
            DI::NetAddress mRemoteAddress;

            // ===== Socket =================================================
            virtual ~Socket_Client();

        protected:

            // ===== Socket =================================================
            virtual void VerifyState_CLOSED(State aS);
            virtual void VerifyState_OPEN  (State aS);

        private:

            void Connect_Internal();

        };

    }
}
