
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Client.cpp

// TEST COVERAGE  2023-11-12  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/HTTP/Result.h>
#include <KMS/Net/Socket_Client_TLS.h>

#include <KMS/HTTP/Client.h>

KMS_RESULT_STATIC(RESULT_HTTP_ERROR);

// Data type
// //////////////////////////////////////////////////////////////////////////

// Constants
// //////////////////////////////////////////////////////////////////////////

#define EOL "\r\n"

#define URL_LENGTH (2048)

class Header
{

public:

    Header();

    unsigned int GetContentLength() const;

    const char* GetLocation() const;

    KMS::HTTP::Result GetResult() const;

    const char* GetResultName() const;

    unsigned int GetSize() const;

    bool Parse(const char* aIn, unsigned int aInSize_byte);

private:

    unsigned int mContentLength_byte;
    unsigned int mResult;
    unsigned int mSize_byte;

    char mLocation[URL_LENGTH];

};

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

            char lBuffer[8192];

            sprintf_s(lBuffer,
                "GET /%s HTTP/1.1" EOL
                "Host: %s" EOL
                "Connection: keep - alive" EOL
                "User-Agent: KMS-Framework" EOL
                "Accept: text/html" EOL
                "Accept-Encoding: deflate" EOL
                EOL,
                lPath,
                lHost);

            auto lSize_byte = static_cast<unsigned int>(strlen(lBuffer));

            mSocket->Send(lBuffer, lSize_byte);

            memset(&lBuffer, 0, sizeof(lBuffer));

            Header lHeader;
            unsigned int lReceived_byte = 0;

            do
            {
                lReceived_byte += mSocket->Receive(lBuffer + lReceived_byte, sizeof(lBuffer) - lReceived_byte);
            }
            while (!lHeader.Parse(lBuffer, lReceived_byte));

            switch (lHeader.GetResult())
            {
            case HTTP::Result::OK:
                // Write the part we already received
                lSize_byte = lReceived_byte - lHeader.GetSize();
                aOutFile->Write(lBuffer + lHeader.GetSize(), lSize_byte);

                ReceiveRestOfFile(lHeader.GetContentLength() - lSize_byte, aOutFile);

                mSocket->Disconnect();
                break;

            case HTTP::Result::FOUND:
                KMS_DBG_LOG_INFO();
                Dbg::gLog.WriteMessage(lHeader.GetLocation());

                mSocket->Disconnect();

                Get(lHeader.GetLocation(), aOutFile);
                break;

            default: KMS_EXCEPTION(RESULT_HTTP_ERROR, "HTTP Error", lHeader.GetResultName());
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

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

Header::Header() : mContentLength_byte(0), mResult(0), mSize_byte(0)
{
    memset(&mLocation, 0, sizeof(mLocation));
}

unsigned int Header::GetContentLength() const { return mContentLength_byte; }
const char * Header::GetLocation     () const { return mLocation; }
HTTP::Result Header::GetResult       () const { return static_cast<HTTP::Result>(mResult); }
const char * Header::GetResultName   () const { return HTTP::GetResultName(mResult); }
unsigned int Header::GetSize         () const { return mSize_byte; }

bool Header::Parse(const char* aIn, unsigned int aInSize_byte)
{
    assert(nullptr != aIn);

    KMS_EXCEPTION_ASSERT(4 < aInSize_byte, RESULT_INVALID_FORMAT, "Invalid HTTP response size", aInSize_byte);

    char lDump[URL_LENGTH];

    auto lRet = sscanf_s(aIn, "HTTP/1.1 %u %s" EOL, &mResult, lDump SizeInfo(lDump));
    KMS_EXCEPTION_ASSERT(2 == lRet, RESULT_INVALID_FORMAT, "Invalid HTTP response format", lRet);

    auto lInSize_byte = aInSize_byte - 2;

    for (unsigned int i = 10; i < lInSize_byte; i++)
    {
        if ('\n' == aIn[i])
        {
            auto lPtr = aIn + i + 1;

            if (0 == strncmp(lPtr, EOL, 2))
            {
                mSize_byte = i + 3;
                return true;
            }

            lRet = sscanf_s(lPtr, "Content-Length: %u" EOL, &mContentLength_byte);
            lRet = sscanf_s(lPtr, "Location: %[^\r\n]" EOL, mLocation SizeInfo(mLocation));
        }
    }

    return false;
}
