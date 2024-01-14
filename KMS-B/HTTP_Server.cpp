
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/HTTP/HTTP.h>
#include <KMS/HTTP/Transaction.h>

#include <KMS/HTTP/Server.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_LOCAL_PORT (80)

// Constants
// //////////////////////////////////////////////////////////////////////////

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Server::Server()
        {
            mResponseHeader.AddConstEntry(Response::FIELD_NAME_SERVER, &Response::FIELD_VALUE_SERVER);

            mSocket.SetLocalPort(DEFAULT_LOCAL_PORT);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Server::OnConnect(Net::Socket* aSocket)
        {
            try
            {
                Transaction lTransaction(aSocket, true);

                lTransaction.mResponse_Header += mResponseHeader;

                if (lTransaction.Request_Receive())
                {
                    auto lRet = mOnRequest.Send(this, &lTransaction);
                    if (CALLBACK_SUCCESS(lRet))
                    {
                        lTransaction.Response_Reply();
                    }
                }
            }
            catch (Exception eE)
            {
                if (RESULT_RECEIVE_FAILED != eE.GetCode())
                {
                    throw eE;
                }
            }
        }

    }
}
