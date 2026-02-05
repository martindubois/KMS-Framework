
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2026 KMS
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
#include <KMS/Ptr.h>

namespace KMS
{
    namespace HTTP
    {

        /// \brief TODO Remove use of DI namespace
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

            // aSocket  The value passed here is reset to nullptr when the
            //          method returns.
            Transaction(Ptr_OF<Net::Socket>& aSocket);

            ~Transaction();

            const char* GetPath() const;

            Type GetType() const;

            const char* GetTypeName() const;

            // aF  The value passed here is reset to nullptr when the method
            //     returns.
            void SetFile(Ptr_OF<File::Binary>& aF);

            // ===== Request ================================================

            const DI::Object* GetRequestData() const;

            void SetPath(const char* aP);

            // aData  The value passed here is reset to nullptr when the
            //        method returns.
            void SetRequestData(Ptr_OF<DI::Object>& aData);

            void SetType(Type aType);

            DI::Dictionary mRequest_Header;

            // ===== Response ===============================================

            const DI::Object* DetachResponseData();

            Result GetResult() const;

            void SetData(const void* aData, unsigned int aDataSize_byte);

            // aData  The value passed here is reset to nullptr when the
            //        method returns.
            void SetResponseData(Ptr_OF<DI::Object>& aData);

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

            bool Request_Parse(unsigned int* aHeaderSize_byte);

            void Response_Parse(unsigned int aSize_byte);

            void RetrieveTime(char* aOut, unsigned int aOutSize_byte);

            char         mBuffer[8192];
            unsigned int mBuffer_byte;

            Ptr_OF<File::Binary> mFile;

            unsigned int mMajor;
            unsigned int mMinor;

            String_ASCII mPath;

            Ptr_OF<Net::Socket> mSocket;

            Type mType;

            // ===== Request ================================================
            Ptr_OF<DI::Object> mRequest_Data;

            // ===== Response ===============================================

            const void * mData;
            unsigned int mDataSize_byte;

            Ptr_OF<DI::Object> mResponse_Data;

            Result mResult;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Transaction& aR);

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Transaction::Type& aT);
