
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Client.cpp

// TEST COVERAGE  2023-11-12  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/HTTP/HTTP.h>
#include <KMS/HTTP/Transaction.h>
#include <KMS/Net/Socket_Client_TLS.h>

#include <KMS/HTTP/Client.h>

KMS_RESULT_STATIC(RESULT_HTTP_ERROR);

// Constants
// //////////////////////////////////////////////////////////////////////////

#define URL_LENGTH (2048)

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Client::Client(bool aSecure) : mSecure(aSecure)
        {
            if (aSecure)
            {
                mSocket = new Net::Socket_Client_TLS;
            }
            else
            {
                mSocket = new Net::Socket_Client(Net::Socket::Type::STREAM);
            }

            mSocket->mLocalAddress.Set("IPv4");
            mSocket->mNoDelay           = true;
            mSocket->mReceiveTimeout_ms = 10000; // 10 s
        }

        Client::~Client()
        {
            assert(nullptr != mSocket);

            delete mSocket;
        }

        void Client::Get(const char* aURL, File::Binary* aOutFile)
        {
            assert(nullptr != aURL);
            assert(nullptr != aOutFile);

            KMS_DBG_LOG_NOISE();
            Dbg::gLog.WriteMessage(aURL);

            char lHost[NAME_LENGTH];
            char lPath[URL_LENGTH];

            int lRet;

            if (mSecure)
            {
                lRet = sscanf_s(aURL, "https://%[^/]/%s", lHost SizeInfo(lHost), lPath SizeInfo(lPath));
            }
            else
            {
                lRet = sscanf_s(aURL, "http://%[^/]/%s", lHost SizeInfo(lHost), lPath SizeInfo(lPath));
            }
            KMS_EXCEPTION_ASSERT(2 == lRet, RESULT_INVALID_FORMAT, "Invalid URL format", aURL);

            ConnectSocket(lHost);

            Transaction lTransaction(mSocket, false);

            lTransaction.SetType(Transaction::Type::GET);
            lTransaction.SetPath(lPath);

            lTransaction.mRequest_Header.AddEntry     (Request::FIELD_NAME_HOST           , new DI::String(lHost), true);
            lTransaction.mRequest_Header.AddConstEntry(Request::FIELD_NAME_CONNECTION     , &Request::FIELD_VALUE_CONNECTION);
            lTransaction.mRequest_Header.AddConstEntry(Request::FIELD_NAME_USER_AGENT     , &Request::FIELD_VALUE_USER_AGENT);
            lTransaction.mRequest_Header.AddConstEntry(Request::FIELD_NAME_ACCEPT         , &Request::FIELD_VALUE_ACCEPT_TEXT_HTML);
            lTransaction.mRequest_Header.AddConstEntry(Request::FIELD_NAME_ACCEPT_ENCODING, &Request::FIELD_VALUE_ACCEPT_ENCODING_DEFLATE);

            lTransaction.Request_Send();

            lTransaction.SetFile(aOutFile, false);

            lTransaction.Response_Receive();

            switch (lTransaction.GetResult())
            {
            case HTTP::Result::OK: break;

            case HTTP::Result::FOUND:
                {
                    auto lObj = lTransaction.mResponse_Header.GetEntry_R(Response::FIELD_NAME_LOCATION);
                    KMS_EXCEPTION_ASSERT(nullptr != lObj, RESULT_HTTP_ERROR, "Redirection without location", "");

                    auto lLocation = dynamic_cast<const DI::String*>(lObj);
                    KMS_EXCEPTION_ASSERT(nullptr != lLocation, RESULT_HTTP_ERROR, "Redirection format error", "");

                    KMS_DBG_LOG_INFO();
                    Dbg::gLog.WriteMessage(lLocation->Get());

                    mSocket->Disconnect();

                    // TODO  Protect against infinite recursion
                    Get(lLocation->Get(), aOutFile);
                }
                break;

            default: assert(false);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Client::ConnectSocket(const char* aHost)
        {
            assert(nullptr != aHost);

            char lRA[LINE_LENGTH];

            if (mSecure)
            {
                sprintf_s(lRA, "IPv4:%s:443", aHost);

                auto lSocket = dynamic_cast<Net::Socket_Client_TLS*>(mSocket);
                assert(nullptr != lSocket);

                lSocket->mRemoteName = aHost;
            }
            else
            {
                sprintf_s(lRA, "IPv4:%s:80", aHost);
            }

            mSocket->mRemoteAddress = lRA;

            mSocket->Connect();
        }

        void Client::ReceiveRestOfFile(unsigned int aToRecv_byte, File::Binary* aOutFile)
        {
            assert(nullptr != aOutFile);

            auto lToRecv_byte = aToRecv_byte;

            auto lRest = new uint8_t[lToRecv_byte];

            while (0 < lToRecv_byte)
            {
                auto lSize_byte = mSocket->Receive(lRest, lToRecv_byte);

                aOutFile->Write(lRest, lSize_byte);

                lToRecv_byte -= lSize_byte;
            }

            delete[] lRest;

            aOutFile->Close();
        }

    }
}
