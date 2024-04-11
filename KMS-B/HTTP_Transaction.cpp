
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Transaction.cpp

// TEST COVERAGE 2023-12-12 Martin Dubois

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

        const bool Transaction::GetValue_UInt32(const DI::Dictionary& aDictionary, const char* aName, uint32_t* aOut)
        {
            assert(nullptr != aName);
            assert(nullptr != aOut);

            bool lResult = false;

            auto lUInt = aDictionary.GetEntry_R<DI::UInt<uint32_t>>(aName);
            if (nullptr != lUInt)
            {
                *aOut = lUInt->Get();
                lResult = true;
            }

            return lResult;
        }

        const char* Transaction::GetValue_String(const DI::Dictionary& aDictionary, const char* aName)
        {
            assert(nullptr != aName);

            const char* lResult = nullptr;

            auto lString = aDictionary.GetEntry_R<DI::String>(aName);
            if (nullptr != lString)
            {
                lResult = lString->Get();
            }

            return lResult;
        }

        Transaction::Transaction(Ptr_OF<Net::Socket>& aSocket)
            : mData(nullptr)
            , mDataSize_byte(0)
            , mMajor(1)
            , mMinor(1)
            , mResult(Result::OK)
            , mSocket(aSocket)
            , mType(Type::GET)
        {
            Buffer_Clear();
        }

        Transaction::~Transaction() {}

        const char* Transaction::GetPath() const { return mPath.c_str(); }

        Transaction::Type Transaction::GetType() const { return mType; }

        const char* Transaction::GetTypeName() const
        {
            assert(Type::QTY > mType);

            Enum<Type, TYPE_NAMES> lType(mType);

            return lType.GetName();
        }

        void Transaction::SetFile(Ptr_OF<File::Binary>& aF) { mFile.Set(aF); }

        // ===== Request ====================================================

        const DI::Object* Transaction::GetRequestData() const { return mRequest_Data; }

        void Transaction::SetPath(const char* aP) { assert(nullptr != aP); mPath = aP; }

        void Transaction::SetRequestData(Ptr_OF<DI::Object>& aData) { mRequest_Data = aData; }

        void Transaction::SetType(Type aT) { mType = aT; }

        // ===== Response ===================================================

        const DI::Object* Transaction::DetachResponseData() { return mResponse_Data.Detach(); }

        Result Transaction::GetResult() const { return mResult; }

        void Transaction::SetData(const void* aData, unsigned int aDataSize_byte)
        {
            mData          = aData;
            mDataSize_byte = aDataSize_byte;
        }

        void Transaction::SetResult(Result aR) { mResult = aR; }

        void Transaction::SetResponseData(Ptr_OF<DI::Object>& aData)
        {
            mResponse_Data = aData;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

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

            auto lResult = Buffer_Receive();
            if (lResult)
            {
                KMS_DBG_LOG_INFO();
                Dbg::gLog.WriteMessage(mBuffer);

                unsigned int lHeaderSize_byte;

                lResult = Request_Parse(&lHeaderSize_byte);
                if (lResult)
                {
                    Buffer_Discard(lHeaderSize_byte);

                    DI::Object* lRequestData = nullptr;

                    JSON_ReceiveAndDecode(mRequest_Header, &lRequestData);

                    mRequest_Data.Set(lRequestData, true);
                }
            }

            return lResult;
        }

        void Transaction::Request_Send()
        {
            JSON_Encode(&mRequest_Header, mRequest_Data);

            auto lSize_byte = sprintf_s(mBuffer,
                "%s /%s HTTP/1.1" HTTP_EOL,
                GetTypeName(),
                mPath.c_str());

            lSize_byte += HTTP::Encode_Dictionary(&mRequest_Header, mBuffer + lSize_byte, sizeof(mBuffer) - lSize_byte);

            mSocket->Send(mBuffer, lSize_byte);

            Data_Send();
        }

        void Transaction::Response_Receive()
        {
            auto lHeader_byte = Header_Receive();
                
            Response_Parse(lHeader_byte);

            Buffer_Discard(lHeader_byte + HTTP_EOH_LENGTH);

            DI::Object* lResponse_Data = nullptr;

            JSON_ReceiveAndDecode(mResponse_Header, &lResponse_Data);

            mResponse_Data.Set(lResponse_Data, true);

            switch (mResult)
            {
            case Result::OK:
                if (nullptr != mFile)
                {
                    if (0 < mBuffer_byte)
                    {
                        mFile->Write(mBuffer, mBuffer_byte);
                    }

                    uint32_t lLength_byte;

                    auto lRet = GetValue_UInt32(mResponse_Header, FIELD_NAME_CONTENT_LENGTH, &lLength_byte);
                    if (lRet && (mBuffer_byte < lLength_byte))
                    {
                        File_Receive(lLength_byte - mBuffer_byte);
                    }
                }
                break;

            case Result::FOUND: break;

            default: KMS_EXCEPTION(RESULT_HTTP_ERROR, "HTTP Error (NOT TESTED)", GetResultName());
            }
        }

        void Transaction::Response_Reply()
        {
            JSON_Encode(&mResponse_Header, mResponse_Data);

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

            Data_Send();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Transaction::Buffer_Clear()
        {
            memset(&mBuffer, 0, sizeof(mBuffer));

            mBuffer_byte = 0;
        }

        void Transaction::Buffer_Discard(unsigned int aSize_byte)
        {
            assert(mBuffer_byte >= aSize_byte);

            if (mBuffer_byte == aSize_byte)
            {
                Buffer_Clear();
            }
            else
            {
                mBuffer_byte -= aSize_byte;

                memmove(mBuffer, mBuffer + aSize_byte, mBuffer_byte);
                memset(mBuffer + mBuffer_byte, 0, sizeof(mBuffer) - mBuffer_byte);
            }
        }

        bool Transaction::Buffer_Receive(unsigned int aSize_byte)
        {
            bool lResult = true;

            if (0 < aSize_byte)
            {
                lResult = false;

                unsigned int lSize_byte = sizeof(mBuffer) - mBuffer_byte - 1;
                if (0 < lSize_byte)
                {
                    if (0xffffffff != aSize_byte)
                    {
                        if (lSize_byte < aSize_byte) { return false; }

                        lSize_byte = aSize_byte;
                    }

                    auto lRet_byte = mSocket->Receive(mBuffer + mBuffer_byte, lSize_byte);
                    if ((0 < lRet_byte) && (lSize_byte >= lRet_byte))
                    {
                        mBuffer_byte += lRet_byte;
                        lResult = true;
                    }
                }
            }

            return lResult;
        }

        void Transaction::Data_Send()
        {
            assert(nullptr != mSocket);

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

        // Return  Header size (in byte) without the end of header mark
        //         (HTTP_EOH_LENGTH)
        unsigned int Transaction::Header_Receive()
        {
            Buffer_Clear();

            char* lEnd;

            do
            {
                auto lRet = Buffer_Receive();
                KMS_EXCEPTION_ASSERT(lRet, RESULT_RECEIVE_FAILED, "Socket::Receive failed", "");
            }
            while (nullptr == (lEnd = strstr(mBuffer, HTTP_EOH)));

            return static_cast<unsigned int>(lEnd - mBuffer);
        }

        void Transaction::JSON_Encode(DI::Dictionary* aHeader, const DI::Object* aIn)
        {
            assert(nullptr != aHeader);

            if (nullptr != aIn)
            {
                char lData[8192];

                auto lDictionary = dynamic_cast<const DI::Dictionary*>(aIn);
                assert(nullptr != lDictionary);

                auto lSize_byte = JSON::Encode_Dictionary(lDictionary, lData, sizeof(lData));

                Ptr_OF<DI::Object> lEntry;

                lEntry.Set(new DI::UInt<uint32_t>(lSize_byte), true); aHeader->AddEntry(FIELD_NAME_CONTENT_LENGTH, lEntry);

                lEntry.Set(&FIELD_VALUE_CONTENT_TYPE_APPLICATION_JSON); aHeader->AddEntry(FIELD_NAME_CONTENT_TYPE, lEntry);

                SetData(lData, lSize_byte);
            }
        }

        void Transaction::JSON_ReceiveAndDecode(const DI::Dictionary& aHeader, DI::Object** aOut)
        {
            assert(nullptr != aOut);

            auto lType = GetValue_String(aHeader, HTTP::FIELD_NAME_CONTENT_TYPE);
            if ((nullptr != lType) && (0 == strcmp(lType, HTTP::FIELD_VALUE_CONTENT_TYPE_APPLICATION_JSON)))
            {
                uint32_t lLength_byte = 0;

                auto lRet = GetValue_UInt32(aHeader, HTTP::FIELD_NAME_CONTENT_LENGTH, &lLength_byte);
                if ((!lRet) && (0 < mBuffer_byte))
                {
                    char* lEnd = nullptr;

                    lLength_byte = strtoul(mBuffer, &lEnd, 16);
                    assert(nullptr != lEnd);

                    auto lParsed_byte = static_cast<unsigned int>(lEnd - mBuffer) + 4;

                    Buffer_Discard(lParsed_byte);
                }

                if (0 < lLength_byte)
                {
                    bool lRet = true;

                    if (lLength_byte > mBuffer_byte)
                    {
                        auto lToRead_byte = lLength_byte - mBuffer_byte;

                        lRet = Buffer_Receive(lToRead_byte);
                    }

                    if (lRet)
                    {
                        auto lRet_byte = JSON::Decode(aOut, mBuffer, lLength_byte);
                        assert(lLength_byte >= lRet_byte);

                        Buffer_Discard(lLength_byte);
                    }
                }
            }
        }

        // Return  false  NOT TESTED
        bool Transaction::Request_Parse(unsigned int* aHeaderSize_byte)
        {
            assert(nullptr != aHeaderSize_byte);

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

            auto lPtr = strstr(mBuffer, HTTP_EOL);
            if (nullptr == lPtr) { return false; }

            auto lEnd = strstr(lPtr, HTTP_EOH);
            if (nullptr == lEnd) { return false; }

            auto lIndex = static_cast<unsigned int>((lPtr - mBuffer) + HTTP_EOL_LENGTH);
            auto lSize_byte = static_cast<unsigned int>(lEnd - lPtr) - HTTP_EOL_LENGTH;

            *aHeaderSize_byte = static_cast<unsigned int>(lEnd - mBuffer) + HTTP_EOH_LENGTH;

            HTTP::Decode_Dictionary(&mRequest_Header, mBuffer + lIndex,lSize_byte);

            return true;
        }

        void Transaction::Response_Parse(unsigned int aSize_byte)
        {
            assert(0 < aSize_byte);

            char lResultName[NAME_LENGTH];

            auto lRet = sscanf_s(mBuffer, "HTTP/%u.%u %u %[^\n\r]", &mMajor, &mMinor, reinterpret_cast<unsigned int*>(&mResult), lResultName SizeInfo(lResultName));
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
