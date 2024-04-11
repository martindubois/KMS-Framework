
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

#define DEFAULT_LOCAL_PORT (0)

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
            Ptr_OF<DI::Object> lEntry(&Response::FIELD_VALUE_SERVER);

            mResponseHeader.AddEntry(Response::FIELD_NAME_SERVER, lEntry);

            mSocket.SetLocalPort(DEFAULT_LOCAL_PORT);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Server::OnConnect(Net::Socket* aSocket)
        {
            try
            {
                Ptr_OF<Net::Socket> lSocket(aSocket, true);

                Transaction lTransaction(lSocket);

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
