
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/HTTP/Request.h>

#include <KMS/HTTP/Server.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_LOCAL_PORT (80)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI::MetaData MD_SERVER("Server", NULL);

static const KMS::DI::String SERVER("KMS-Framework", &MD_SERVER);

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Server::Server()
        {
            mResponseHeader += const_cast<DI::String*>(&SERVER);

            mSocket.SetLocalPort(DEFAULT_LOCAL_PORT);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Server::OnConnect(Net::Socket* aSocket)
        {
            try
            {
                Request lRequest(aSocket);

                lRequest.mResponseHeader = mResponseHeader;

                if (lRequest.Receive())
                {
                    unsigned int lRet = mOnRequest.Send(this, &lRequest);
                    if (KMS_MSG_SUCCESS(lRet))
                    {
                        lRequest.Reply();
                    }
                }
            }
            catch (Exception eE)
            {
                switch (eE.GetCode())
                {
                case Exception::Code::SOCKET_RECEIVE: break;

                default: throw eE;
                }
            }
        }

    }
}
