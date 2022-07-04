
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Server.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define ACCEPT_TIMEOUT_ms (1000)

#define CODE_ON_ITERATE  (1)
#define CODE_ON_START    (2)
#define CODE_ON_STOP     (3)
#define CODE_ON_STOPPING (4)

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // ===== Msg::IReceiver =============================================

        bool Server::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            assert(&mThread == aSender);

            bool lResult = false;

            switch (aCode)
            {
            case CODE_ON_ITERATE : lResult = OnIterate (); break;
            case CODE_ON_START   : lResult = OnStarting(); break;
            case CODE_ON_STOP    : lResult = OnStop    (); break;
            case CODE_ON_STOPPING: lResult = OnStopping(); break;

            default: assert(false);
            }

            return lResult;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Server::Server() : mSocket(Net::Socket::Type::STREAM)
        {
            mThread.mOnIterate .Set(this, CODE_ON_ITERATE );
            mThread.mOnStarting.Set(this, CODE_ON_START   );
            mThread.mOnStop    .Set(this, CODE_ON_STOP    );
            mThread.mOnStopping.Set(this, CODE_ON_STOPPING);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Server::OnIterate()
        {
            bool lResult = true;

            Net::Address lFrom;

            Net::Socket* lSocket = mSocket.Accept(ACCEPT_TIMEOUT_ms, &lFrom);
            if (NULL != lSocket)
            {
                OnConnect(lSocket);
            }

            return true;
        }

        bool Server::OnStarting()
        {
            Net::Thread_Startup();

            return true;
        }

        bool Server::OnStop()
        {
            mSocket.Close();

            return true;
        }

        bool Server::OnStopping()
        {
            Net::Thread_Cleanup();

            return true;
        }

    }
}
