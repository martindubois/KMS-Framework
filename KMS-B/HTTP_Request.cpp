
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Request.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/HTTP/HTTP.h>
#include <KMS/JSON/JSON.h>

#include <KMS/HTTP/Request.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define NAME_CONTENT_LENGTH "content-length"
#define NAME_CONTENT_TYPE   "content-type"

static const KMS::DI::String APPLICATION_JSON("application/json");

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Request::Request() : mSocket(nullptr) { Construct(); }

        const char* Request::GetPath() const { return mPath.c_str(); }

        const DI::Dictionary& Request::GetRequestHeader() const { return mRequestHeader; }

        Request::Type Request::GetType() const { return mType; }

        const char* Request::GetTypeName() const
        {
            const char* lResult = nullptr;

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

        void Request::SetFile(File::Binary* aF) { assert(nullptr != aF); mFile = aF; }

        void Request::SetResult(Result aR) { mResult = aR; }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Request::Request(Net::Socket* aSocket) : mSocket(aSocket)
        {
            assert(nullptr != aSocket);

            Construct();
        }

        Request::~Request()
        {
            assert(nullptr != mSocket);

            if (nullptr != mFile)
            {
                delete mFile;
            }

            delete mSocket;
        }

        void Request::Construct()
        {
            memset(&mBuffer, 0, sizeof(mBuffer));

            mData          = nullptr;
            mDataSize_byte = 0;
            mFile          = nullptr;
            mMajor         = 0;
            mMinor         = 0;
            mResult        = Result::OK;

            mRequestHeader.SetCreator(DI::String::Create);
        }

        const char* Request::GetResultName() const
        {
            return HTTP::GetResultName(mResult);
        }

        void Request::Display(std::ostream& aOut) const
        {
            aOut << "\n";
            aOut << GetTypeName() << " " << mPath.c_str() << "\n";
            aOut << GetResultName() << "\n";
            aOut << std::endl;
        }

        bool Request::Receive()
        {
            auto lSize_byte = mSocket->Receive(mBuffer, sizeof(mBuffer) - 1);
            if ((0 >= lSize_byte) || (sizeof(mBuffer) <= lSize_byte))
            {
                return false;
            }

            return Parse();
        }

        void Request::Reply()
        {
            static const char* DAY_NAMES[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
            static const char* MONTH_NAMES[13] = { nullptr, "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dev" };

            char lData[1024];

            if (!mResponseData.IsEmpty())
            {
                unsigned int lSize_byte = JSON::Encode_Dictionary(&mResponseData, lData, sizeof(lData));

                mResponseHeader.AddEntry(NAME_CONTENT_LENGTH, new DI::UInt<uint32_t>(lSize_byte), true);

                mResponseHeader.AddConstEntry(NAME_CONTENT_TYPE, &APPLICATION_JSON);

                SetData(lData, lSize_byte);
            }

            SYSTEMTIME lST;

            GetSystemTime(&lST);

            auto lSize_byte = sprintf_s(mBuffer,
                "HTTP/1.1 %u %s\r\n"
                "Date: %s, %u %s %u %u:%02u:%02u GMT\r\n",
                mResult, GetResultName(),
                DAY_NAMES[lST.wDayOfWeek], lST.wDay, MONTH_NAMES[lST.wMonth], lST.wYear, lST.wHour, lST.wMinute, lST.wSecond);

            lSize_byte += HTTP::Encode_Dictionary(&mResponseHeader, mBuffer + lSize_byte, sizeof(mBuffer) - lSize_byte);

            mSocket->Send(mBuffer, lSize_byte);

            if (0 < mDataSize_byte)
            {
                assert(nullptr != mData);

                mSocket->Send(mData, mDataSize_byte);
            }
            else if (nullptr != mFile)
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

            auto lPtr = strchr(mBuffer, '\n');
            assert(nullptr != lPtr);

            auto lIndex = static_cast<unsigned int>((lPtr - mBuffer) + 1);

            HTTP::Decode_Dictionary(&mRequestHeader, mBuffer + lIndex, sizeof(mBuffer) - lIndex);

            return true;
        }

    }
}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const HTTP::Request& aR)
{
    aR.Display(aOut);

    return aOut;
}
