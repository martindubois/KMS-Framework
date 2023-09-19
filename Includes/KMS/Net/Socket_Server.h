
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Socket_Server.h
// Library   KMS-B

#pragma once

// ===== Includes ========================================================
#include <KMS/Net/Socket.h>

namespace KMS
{
    namespace Net
    {

        class Socket_Server : public Socket
        {

        public:

            DI::Array mAllowedRanges;

            Socket_Server(Type aType);

            Socket * Accept(unsigned int aTimeout_ms, Address* aFrom);

            // ===== Socket =================================================
            virtual ~Socket_Server();

        protected:

            // ===== Socket =================================================
            virtual void VerifyState       (State aS);
            virtual void VerifyState_CLOSED(State aS);
            virtual void VerifyState_LISTEN(State aS);
            virtual void VerifyState_OPEN  (State aS);

        private:

            bool IsInAllowedRanges(const Address& aA) const;

            void Listen();

        };

    }
}
