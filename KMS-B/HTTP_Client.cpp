
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
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

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

static void InitTransaction(KMS::HTTP::Transaction* aTransaction);

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Client::Client(bool aSecure) : mResponse_Data(nullptr), mSecure(aSecure)
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

            Response_DeleteData();

            delete mSocket;
        }

        const DI::Object* Client::GetResponseData() const { return mResponse_Data; }

        void Client::Get(const char* aURL, File::Binary* aOutFile)
        {
            char lHost[NAME_LENGTH];
            char lPath[URL_LENGTH];

            ParseURL(aURL, lHost, sizeof(lHost), lPath, sizeof(lPath));

            ConnectSocket(lHost);

            Transaction lTransaction(mSocket, false);

            InitTransaction(&lTransaction);

            lTransaction.SetType(Transaction::Type::GET);
            lTransaction.SetPath(lPath);

            lTransaction.mRequest_Header.AddEntry(Request::FIELD_NAME_HOST, new DI::String(lHost), true);

            lTransaction.Request_Send();

            if (nullptr != aOutFile)
            {
                lTransaction.SetFile(aOutFile, false);
            }

            lTransaction.Response_Receive();

            auto lData = lTransaction.DetachResponseData();
            if (nullptr != lData)
            {
                Response_SetData(lData);
            }

            switch (lTransaction.GetResult())
            {
            case HTTP::Result::OK: break;

            case HTTP::Result::FOUND:
                {
                    auto lLocation = lTransaction.mResponse_Header.GetEntry_R<DI::String>(Response::FIELD_NAME_LOCATION);
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

        void Client::Post(const char* aURL, const DI::Object* aData)
        {
            char lHost[NAME_LENGTH];
            char lPath[URL_LENGTH];

            ParseURL(aURL, lHost, sizeof(lHost), lPath, sizeof(lPath));

            ConnectSocket(lHost);

            Transaction lTransaction(mSocket, false);

            InitTransaction(&lTransaction);

            lTransaction.SetType(Transaction::Type::POST);
            lTransaction.SetPath(lPath);

            lTransaction.mRequest_Header.AddEntry(Request::FIELD_NAME_HOST, new DI::String(lHost), true);

            if (nullptr != aData)
            {
                lTransaction.SetRequestData(const_cast<DI::Object*>(aData), false);
            }

            lTransaction.Request_Send();

            lTransaction.Response_Receive();

            auto lData = lTransaction.DetachResponseData();
            if (nullptr != lData)
            {
                Response_SetData(lData);
            }

            switch (lTransaction.GetResult())
            {
            case HTTP::Result::OK: break;

            case HTTP::Result::FOUND: KMS_EXCEPTION(RESULT_NOT_IMPLEMENTED, "Redirection is not supported for POST request", ""); break;

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

        void Client::ParseURL(const char* aURL, char* aHost, unsigned int aHostSize_byte, char* aPath, unsigned int aPathSize_byte)
        {
            assert(nullptr != aURL);
            assert(nullptr != aHost);
            assert(0 < aHostSize_byte);
            assert(nullptr != aPath);
            assert(0 < aPathSize_byte);

            KMS_DBG_LOG_NOISE();
            Dbg::gLog.WriteMessage(aURL);

            int lRet;

            if (mSecure)
            {
                lRet = sscanf_s(aURL, "https://%[^/]/%s", aHost SizeInfoV(aHostSize_byte), aPath SizeInfoV(aPathSize_byte));
            }
            else
            {
                lRet = sscanf_s(aURL, "http://%[^/]/%s", aHost SizeInfoV(aHostSize_byte), aPath SizeInfoV(aPathSize_byte));
            }
            KMS_EXCEPTION_ASSERT(2 == lRet, RESULT_INVALID_FORMAT, "Invalid URL format", aURL);
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

        void Client::Response_DeleteData()
        {
            if (nullptr != mResponse_Data)
            {
                delete mResponse_Data;
                mResponse_Data = nullptr;
            }
        }

        void Client::Response_SetData(const DI::Object* aData)
        {
            assert(nullptr != aData);

            Response_DeleteData();

            mResponse_Data = aData;
        }

    }
}

using namespace KMS;

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

void InitTransaction(HTTP::Transaction* aTransaction)
{
    assert(nullptr != aTransaction);

    aTransaction->mRequest_Header.AddConstEntry(HTTP::Request::FIELD_NAME_CONNECTION     , &HTTP::Request::FIELD_VALUE_CONNECTION);
    aTransaction->mRequest_Header.AddConstEntry(HTTP::Request::FIELD_NAME_USER_AGENT     , &HTTP::Request::FIELD_VALUE_USER_AGENT);
    aTransaction->mRequest_Header.AddConstEntry(HTTP::Request::FIELD_NAME_ACCEPT         , &HTTP::Request::FIELD_VALUE_ACCEPT_TEXT_HTML);
    aTransaction->mRequest_Header.AddConstEntry(HTTP::Request::FIELD_NAME_ACCEPT_ENCODING, &HTTP::Request::FIELD_VALUE_ACCEPT_ENCODING_DEFLATE);
}
