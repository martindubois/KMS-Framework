
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Request.cpp

// TEST COVERAGE  2023-12-12  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/HTTP/HTTP.h>
#include <KMS/JSON/JSON.h>

#include <KMS/HTTP/Transaction.h>

KMS_RESULT_STATIC(RESULT_HTTP_ERROR);

// Constants
// //////////////////////////////////////////////////////////////////////////

static const char* TYPE_NAMES[] = { "GET", "OPTIONS", "POST" };

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Transaction::GetPath() const { return mPath.c_str(); }

        Transaction::Type Transaction::GetType() const { return mType; }

        const char* Transaction::GetTypeName() const
        {
            assert(Type::QTY > mType);

            Enum<Type, TYPE_NAMES> lType(mType);

            return lType.GetName();
        }

        void Transaction::SetFile(File::Binary* aF, bool aDelete)
        {
            assert(nullptr != aF);

            mFile = aF;
            mFile_Delete = aDelete;
        }

        // ===== Request ====================================================

        void Transaction::SetPath(const char* aP) { assert(nullptr != aP); mPath = aP; }

        void Transaction::SetType(Type aT) { mType = aT; }

        // ===== Response ===================================================

        Result Transaction::GetResult() const { return mResult; }

        void Transaction::SetData(const void* aData, unsigned int aDataSize_byte)
        {
            mData          = aData;
            mDataSize_byte = aDataSize_byte;
        }

        void Transaction::SetResult(Result aR) { mResult = aR; }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Transaction::Transaction(Net::Socket* aSocket, bool aDelete)
            : mData(nullptr)
            , mDataSize_byte(0)
            , mFile(nullptr)
            , mMajor(1)
            , mMinor(1)
            , mResult(Result::OK)
            , mSocket(aSocket)
            , mSocket_Delete(aDelete)
        {
            assert(nullptr != aSocket);

            memset(&mBuffer, 0, sizeof(mBuffer));

            // mRequest_Header.SetCreator(DI::String::Create);
        }

        Transaction::~Transaction()
        {
            assert(nullptr != mSocket);

            if ((nullptr != mFile) && mFile_Delete)
            {
                delete mFile;
            }

            if (mSocket_Delete)
            {
                delete mSocket;
            }
        }

        const char* Transaction::GetResultName() const { return HTTP::GetResultName(mResult); }

        // NOT TESTED
        void Transaction::Display(std::ostream& aOut) const
        {
            aOut << "\n";
            aOut << GetTypeName() << " " << mPath.c_str() << "\n";
            aOut << GetResultName() << "\n";
            aOut << std::endl;
        }

        bool Transaction::Request_Receive()
        {
            KMS_DBG_LOG_NOISE();

            auto lSize_byte = mSocket->Receive(mBuffer, sizeof(mBuffer) - 1);
            if ((0 >= lSize_byte) || (sizeof(mBuffer) <= lSize_byte))
            {
                return false;
            }

            KMS_DBG_LOG_INFO();
            Dbg::gLog.WriteMessage(mBuffer);

            return Request_Parse();
        }

        void Transaction::Request_Send()
        {
            auto lSize_byte = sprintf_s(mBuffer,
                "%s /%s HTTP/1.1" HTTP_EOL,
                GetTypeName(),
                mPath.c_str());

            lSize_byte += HTTP::Encode_Dictionary(&mRequest_Header, mBuffer + lSize_byte, sizeof(mBuffer) - lSize_byte);

            mSocket->Send(mBuffer, lSize_byte);
        }

        void Transaction::Response_Receive()
        {
            unsigned int lSize_byte;

            auto lHeader_byte = Header_Receive(&lSize_byte);
                
            Response_Parse(lHeader_byte);

            switch (mResult)
            {
            case Result::OK:
                if (nullptr != mFile)
                {
                    lHeader_byte += HTTP_EOH_LENGTH;

                    auto lChunk_byte = lSize_byte - lHeader_byte;
                    if (0 < lChunk_byte)
                    {
                        mFile->Write(mBuffer + lHeader_byte, lChunk_byte);
                    }

                    auto lObj = mResponse_Header.GetEntry_R(Response::FIELD_NAME_CONTENT_LENGTH);
                    if (nullptr != lObj)
                    {
                        auto lContentLength = dynamic_cast<const DI::UInt<uint32_t>*>(lObj);
                        assert(nullptr != lContentLength);

                        File_Receive(lContentLength->Get() - lChunk_byte);
                    }
                }
                break;

            case Result::FOUND: break;

            default: KMS_EXCEPTION(RESULT_HTTP_ERROR, "HTTP Error (NOT TESTED)", GetResultName());
            }
        }

        void Transaction::Response_Reply()
        {
            if (!mResponse_Data.IsEmpty())
            {
                char lData[1024];

                auto lSize_byte = JSON::Encode_Dictionary(&mResponse_Data, lData, sizeof(lData));

                mResponse_Header.AddEntry(Response::FIELD_NAME_CONTENT_LENGTH, new DI::UInt<uint32_t>(lSize_byte), true);

                mResponse_Header.AddConstEntry(Response::FIELD_NAME_CONTENT_TYPE, &Response::FIELD_VALUE_CONTENT_TYPE_APPLICATION_JSON);

                SetData(lData, lSize_byte);
            }

            char lTime[64];

            RetrieveTime(lTime, sizeof(lTime));

            // TODO  Put Date into the dictionary
            auto lSize_byte = sprintf_s(mBuffer,
                "HTTP/1.1 %u %s" HTTP_EOL
                "Date: %s" HTTP_EOL,
                static_cast<unsigned int>(mResult), GetResultName(),
                lTime);

            lSize_byte += HTTP::Encode_Dictionary(&mResponse_Header, mBuffer + lSize_byte, sizeof(mBuffer) - lSize_byte);

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

        // aSize_byte  The size of data to read in byte. This size may be
        //             smaller than the file size because part of data may
        //             have been be received when reciving header.
        // 
        // REVIEW  We could use mBuffer rather than allocating another one
        void Transaction::File_Receive(unsigned int aSize_byte)
        {
            assert(nullptr != mFile);

            auto lToRecv_byte = aSize_byte;

            auto lBuffer = new uint8_t[lToRecv_byte];

            while (0 < lToRecv_byte)
            {
                auto lSize_byte = mSocket->Receive(lBuffer, lToRecv_byte);

                mFile->Write(lBuffer, lSize_byte);

                lToRecv_byte -= lSize_byte;
            }

            delete[] lBuffer;

            mFile->Close();
        }

        // aSize_byte  The function put the total received size there.
        //
        // Return  Header size (in byte) without the end of header mark
        //         (HTTP_EOH_LENGTH)
        unsigned int Transaction::Header_Receive(unsigned int* aSize_byte)
        {
            assert(nullptr != aSize_byte);

            memset(&mBuffer, 0, sizeof(mBuffer));

            char       * lEnd;
            unsigned int lSize_byte = 0;

            do
            {
                auto lToRead_byte = sizeof(mBuffer) - lSize_byte - 1;

                KMS_EXCEPTION_ASSERT(0 < lToRead_byte, RESULT_HTTP_ERROR, "HTTP header too long", lToRead_byte);

                auto lRet_byte = mSocket->Receive(mBuffer + lSize_byte, static_cast<unsigned int>(lToRead_byte));
                KMS_EXCEPTION_ASSERT((0 < lRet_byte) || (sizeof(mBuffer) > lRet_byte), RESULT_RECEIVE_FAILED, "Socket::Receive failed", lRet_byte);

                lSize_byte += lRet_byte;
            }
            while (nullptr == (lEnd = strstr(mBuffer, HTTP_EOH)));

            *aSize_byte = lSize_byte;

            return static_cast<unsigned int>(lEnd - mBuffer);
        }

        // Return  false  NOT TESTED
        bool Transaction::Request_Parse()
        {
            char lPath[PATH_LENGTH];
            char lCmd[1024];

            if (4 != sscanf_s(mBuffer, "%[A-Z] %[^ ] HTTP/%u.%u", lCmd SizeInfo(lCmd), lPath SizeInfo(lPath), &mMajor, &mMinor))
            {
                return false;
            }

            Enum<Type, TYPE_NAMES> lType(mType);

            if (!lType.SetName_Try(lCmd))
            {
                return false;
            }

            mPath = lPath;
            mType = lType;

            auto lPtr = strchr(mBuffer, '\n');
            assert(nullptr != lPtr);

            auto lIndex = static_cast<unsigned int>((lPtr - mBuffer) + 1);

            HTTP::Decode_Dictionary(&mRequest_Header, mBuffer + lIndex, sizeof(mBuffer) - lIndex);

            return true;
        }

        void Transaction::Response_Parse(unsigned int aSize_byte)
        {
            assert(0 < aSize_byte);

            char lResultName[NAME_LENGTH];

            auto lRet = sscanf_s(mBuffer, "HTTP/%u.%u %u %[^\n\r]", &mMajor, &mMinor, &mResult, lResultName SizeInfo(lResultName));
            KMS_EXCEPTION_ASSERT(4 == lRet, RESULT_INVALID_FORMAT, "Invalid HTTP header format", lRet);

            auto lPtr = strchr(mBuffer, '\n');
            assert(nullptr != lPtr);
            lPtr++;

            auto lSize_byte = aSize_byte - static_cast<unsigned int>(lPtr - mBuffer);

            HTTP::Decode_Dictionary(&mResponse_Header, lPtr, lSize_byte);
        }

    }
}

using namespace KMS;

// NOT TESTED
std::ostream& operator << (std::ostream& aOut, const HTTP::Transaction& aR)
{
    aR.Display(aOut);

    return aOut;
}
