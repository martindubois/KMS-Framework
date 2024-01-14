
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Transaction.h
// Library   KMS-B

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/File/Binary.h>
#include <KMS/File/Folder.h>
#include <KMS/HTTP/Result.h>
#include <KMS/Net/Socket.h>

namespace KMS
{
    namespace HTTP
    {

        class Transaction final
        {

        public:

            enum class Type
            {
                GET,
                OPTIONS,
                POST,

                QTY
            };

            static const bool GetValue_UInt32(const DI::Dictionary& aDictionary, const char* aName, uint32_t* aOut);

            static const char* GetValue_String(const DI::Dictionary& aDictionary, const char* aName);

            Transaction(Net::Socket* aSocket, bool aDelete);

            ~Transaction();

            const char* GetPath() const;

            Type GetType() const;

            const char* GetTypeName() const;

            void SetFile(File::Binary* aF, bool aDelete);

            // ===== Request ================================================

            const DI::Dictionary& GetParameters() const;

            void SetPath(const char* aP);

            void SetRequestData(DI::Object* aData, bool aDelete);

            void SetType(Type aType);

            DI::Dictionary mRequest_Header;

            // ===== Response ===============================================

            const DI::Object* DetachResponseData();

            Result GetResult() const;

            void SetData(const void* aData, unsigned int aDataSize_byte);

            void SetResponseData(DI::Object* aData);

            void SetResult(Result aR);

            DI::Dictionary mResponse_Header;

        // Internal

            const char* GetResultName() const;

            void Display(std::ostream& aOut) const;

            bool Request_Receive();

            void Request_Send();

            void Response_Receive();

            void Response_Reply();

        private:

            NO_COPY(Transaction);

            void Buffer_Clear();

            void Buffer_Discard(unsigned int aSize_byte);

            bool Buffer_Receive(unsigned int aSize_byte = 0xffffffff);

            void Data_Send();

            void File_Receive(unsigned int aSize_byte);

            unsigned int Header_Receive();

            void JSON_Encode(DI::Dictionary* aHeader, const DI::Object* aIn);

            void JSON_ReceiveAndDecode(const DI::Dictionary& aHeader, DI::Object** aOut);

            void Request_DeleteData();

            bool Request_Parse(unsigned int* aHeaderSize_byte);

            void Response_DeleteData();

            void Response_Parse(unsigned int aSize_byte);

            void RetrieveTime(char* aOut, unsigned int aOutSize_byte);

            char         mBuffer[8192];
            unsigned int mBuffer_byte;

            File::Binary* mFile;
            bool          mFile_Delete;

            unsigned int mMajor;
            unsigned int mMinor;

            std::string mPath;

            Net::Socket* mSocket;
            bool         mSocket_Delete;

            Type mType;

            // ===== Request ================================================
            DI::Object* mRequest_Data;
            bool        mRequest_Data_Delete;

            // ===== Response ===============================================

            const void * mData;
            unsigned int mDataSize_byte;

            DI::Object* mResponse_Data;

            Result mResult;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Transaction& aR);

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Transaction::Type& aT);
