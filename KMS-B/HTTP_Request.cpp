
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Request.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/SafeAPI.h>

#include <KMS/HTTP/Request.h>

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Request::GetPath() const { return mPath.c_str(); }

        const Dictionary& Request::GetRequestHeader() const { return mRequestHeader; }

        Request::Type Request::GetType() const { return mType; }

        const char* Request::GetTypeName() const
        {
            const char* lResult = NULL;

            switch (mType)
            {
            case Type::GET    : lResult = "GET"    ; break;
            case Type::OPTIONS: lResult = "OPTIONS"; break;
            case Type::POST   : lResult = "POST"   ; break;

            default: assert(false);
            }

            return lResult;
        }

        void Request::SetData(const void* aData, unsigned int aDataSize_byte)
        {
            mData          = aData;
            mDataSize_byte = aDataSize_byte;
        }

        void Request::SetFile(File::Binary* aF) { assert(NULL != aF); mFile = aF; }

        void Request::SetResult(Result aR) { mResult = aR; }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Request::Request(Network::Socket* aSocket)
            : mData(NULL)
            , mDataSize_byte(0)
            , mFile(NULL)
            , mMajor(0)
            , mMinor(0)
            , mResult(Result::OK)
            , mSocket(aSocket)
        {
            assert(NULL != aSocket);

            memset(&mBuffer, 0, sizeof(mBuffer));
        }

        Request::~Request()
        {
            assert(NULL != mSocket);

            if (NULL != mFile)
            {
                delete mFile;
            }

            delete mSocket;
        }

        const char* Request::GetResultName() const
        {
            const char* lResult = NULL;

            switch (mResult)
            {
            case Result::ACCEPTED             : lResult = "ACCEPTED"             ; break;
            case Result::BAD_REQUEST          : lResult = "BAD_REQUEST"          ; break;
            case Result::CREATED              : lResult = "CREATED"              ; break;
            case Result::FORBIDDEN            : lResult = "FORBIDDEN"            ; break;
            case Result::INTERNAL_SERVER_ERROR: lResult = "INTERNAL_SERVER_ERROR"; break;
            case Result::METHOD_NOT_ALLOWED   : lResult = "METHOD_NOT_ALLOWED"   ; break;
            case Result::NOT_ACCEPTABLE       : lResult = "NOT_ACCEPTABLE"       ; break;
            case Result::NOT_FOUND            : lResult = "NOT_FOUND"            ; break;
            case Result::NO_CONTENT           : lResult = "NO_CONTENT"           ; break;
            case Result::OK                   : lResult = "OK"                   ; break;
            case Result::REQUEST_TIMEOUT      : lResult = "REQUEST_TIMEOUT"      ; break;
            case Result::RESET_CONTENT        : lResult = "RESET_CONTENT"        ; break;

            default: assert(false);
            }

            return lResult;
        }

        bool Request::Receive()
        {
            unsigned int lSize_byte = mSocket->Receive(mBuffer, sizeof(mBuffer) - 1);
            if ((0 >= lSize_byte) || (sizeof(mBuffer) <= lSize_byte))
            {
                return false;
            }

            return Parse();
        }

        void Request::Reply()
        {
            static const char* DAY_NAMES[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
            static const char* MONTH_NAMES[13] = { NULL, "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dev" };

            char lData[1024];

            if (!mResponseData.IsEmpty())
            {
                unsigned int lSize_byte = mResponseData.JSON_Get(lData, sizeof(lData));

                mResponseHeader.Set("Content-Length", lSize_byte);
                mResponseHeader.Set("Content-Type", "application/json");

                SetData(lData, lSize_byte);
            }

            SYSTEMTIME lST;

            GetSystemTime(&lST);

            int lSize_byte = sprintf_s(mBuffer,
                "HTTP/1.1 %u %s\r\n"
                "Date: %s, %u %s %u %u:%02u:%02u GMT\r\n",
                mResult, GetResultName(),
                DAY_NAMES[lST.wDayOfWeek], lST.wDay, MONTH_NAMES[lST.wMonth], lST.wYear, lST.wHour, lST.wMinute, lST.wSecond);

            lSize_byte += mResponseHeader.HTTP_Get(mBuffer + lSize_byte, sizeof(mBuffer) - lSize_byte);

            lSize_byte += sprintf_s(mBuffer + lSize_byte, sizeof(mBuffer) - lSize_byte, "\r\n");

            mSocket->Send(mBuffer, lSize_byte);

            if (0 < mDataSize_byte)
            {
                assert(NULL != mData);

                mSocket->Send(mData, mDataSize_byte);
            }
            else if (NULL != mFile)
            {
                mSocket->Send(mFile);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Request::Parse()
        {
            char lPath[PATH_LENGTH];
            char lCmd[1024];

            if (4 != sscanf_s(mBuffer, "%[A-Z] %[^ ] HTTP/%u.%u", lCmd SizeInfo(lCmd), lPath SizeInfo(lPath), &mMajor, &mMinor))
            {
                return false;
            }

            if      (0 == strncmp(lCmd, "GET"    , 3)) { mType = Type::GET; }
            else if (0 == strncmp(lCmd, "OPTIONS", 7)) { mType = Type::OPTIONS; }
            else if (0 == strncmp(lCmd, "POST"   , 4)) { mType = Type::POST; }
            else
            {
                return false;
            }

            mPath = lPath;

            mRequestHeader.HTTP_Set(strchr(mBuffer, '\n') + 1);

            return true;
        }

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Request& aR)
{
    aOut << std::endl;
    aOut << aR.GetTypeName() << " " << aR.GetPath() << std::endl;
    aOut << aR.GetRequestHeader();
    aOut << aR.GetResultName() << std::endl;
    aOut << aR.mResponseHeader;
    aOut << std::endl;

    return aOut;
}
