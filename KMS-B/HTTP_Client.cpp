
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Client.cpp

// TEST COVERAGE  2023-11-12  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Windows ============================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/HTTP/Result.h>

#include <KMS/HTTP/Client.h>

KMS_RESULT_STATIC(RESULT_HTTP_ERROR);

// Data type
// //////////////////////////////////////////////////////////////////////////

typedef struct
{
    unsigned int mContentLength_byte;
    unsigned int mResult;
    unsigned int mSize_byte;
}
Header;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define EOL "\r\n"

// Static function declaration
// //////////////////////////////////////////////////////////////////////////

static void ParseHeader(const char* aIn, unsigned int aInSize_byte, Header* aOut);

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Client::Client() : mSocket(Net::Socket::Type::STREAM)
        {
            mSocket.mLocalAddress.Set("IPv4");
            mSocket.mNoDelay           = true;
            mSocket.mReceiveTimeout_ms = 10000; // 10 s
        }

        void Client::Get(const char* aURL, File::Binary* aOutFile)
        {
            assert(nullptr != aURL);
            assert(nullptr != aOutFile);

            char lHost[NAME_LENGTH];
            char lPath[PATH_LENGTH];

            auto lRet = sscanf_s(aURL, "http://%[^/]/%s", lHost SizeInfo(lHost), lPath SizeInfo(lPath));
            KMS_EXCEPTION_ASSERT(2 == lRet, RESULT_INVALID_FORMAT, "Invalid URL format", aURL);

            ConnectSocket(lHost);

            char lBuffer[4096];

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

            mSocket.Send(lBuffer, lSize_byte);

            memset(&lBuffer, 0, sizeof(lBuffer));

            lSize_byte = mSocket.Receive(lBuffer, sizeof(lBuffer));

            Header lHeader;

            ParseHeader(lBuffer, lSize_byte, &lHeader);

            KMS_EXCEPTION_ASSERT(HTTP::Result::OK == static_cast<HTTP::Result>(lHeader.mResult), RESULT_HTTP_ERROR, "HTTP Error", lHeader.mResult);

            // Write the part we already received
            lSize_byte -= lHeader.mSize_byte;
            aOutFile->Write(lBuffer + lHeader.mSize_byte, lSize_byte);

            ReceiveRestOfFile(lHeader.mContentLength_byte - lSize_byte, aOutFile);

            mSocket.Disconnect();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Client::ConnectSocket(const char* aHost)
        {
            assert(nullptr != aHost);

            char lRA[LINE_LENGTH];

            sprintf_s(lRA, "IPv4:%s:80", aHost);

            mSocket.mRemoteAddress = lRA;

            mSocket.Connect();
        }

        void Client::ReceiveRestOfFile(unsigned int aToRecv_byte, File::Binary* aOutFile)
        {
            assert(nullptr != aOutFile);

            auto lToRecv_byte = aToRecv_byte;

            auto lRest = new uint8_t[lToRecv_byte];

            while (0 < lToRecv_byte)
            {
                auto lSize_byte = mSocket.Receive(lRest, lToRecv_byte);

                aOutFile->Write(lRest, lSize_byte);

                lToRecv_byte -= lSize_byte;
            }

            delete[] lRest;

            aOutFile->Close();
        }

    }
}

using namespace KMS;

// Static function
// //////////////////////////////////////////////////////////////////////////

void ParseHeader(const char* aIn, unsigned int aInSize_byte, Header* aOut)
{
    assert(nullptr != aIn);
    assert(0 < aInSize_byte);
    assert(nullptr != aOut);

    memset(aOut, 0, sizeof(Header));

    char lDump[LINE_LENGTH];

    auto lRet = sscanf_s(aIn, "HTTP/1.1 %u %s" EOL, &aOut->mResult, lDump SizeInfo(lDump));
    KMS_EXCEPTION_ASSERT(2 == lRet, RESULT_INVALID_FORMAT, "Invalid HTTP response format", lRet);

    auto lInSize_byte = aInSize_byte - 2;

    for (unsigned int i = 10; i < lInSize_byte; i++)
    {
        if ('\n' == aIn[i])
        {
            auto lPtr = aIn + i + 1;

            if (0 == strncmp(lPtr, EOL, 2))
            {
                aOut->mSize_byte = i + 2;
                return;
            }

            lRet = sscanf_s(lPtr, "Content-Length: %u" EOL, &aOut->mContentLength_byte);
        }
    }

    KMS_EXCEPTION(RESULT_HTTP_ERROR, "Incomplete HTTP response (NOT TESTED)", aInSize_byte);
}
