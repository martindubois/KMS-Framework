
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
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

            const char* GetPath() const;

            Type GetType() const;

            const char* GetTypeName() const;

            void SetFile(File::Binary* aF, bool aDelete);

            // ===== Request ================================================

            const DI::Dictionary& GetParameters() const;

            void SetPath(const char* aP);

            void SetType(Type aType);

            DI::Dictionary mRequest_Header;

            // ===== Response ===============================================

            void AddData(const DI::Dictionary& aD);
            void AddData(const File::Folder& aF, const char* aFile);

            Result GetResult() const;

            void SetData(const void* aData, unsigned int aDataSize_byte);

            void SetResult(Result aR);

            DI::Dictionary mResponse_Data;
            DI::Dictionary mResponse_Header;

        // Internal

            Transaction(Net::Socket* aSocket, bool aDelete);

            ~Transaction();

            const char* GetResultName() const;

            void Display(std::ostream& aOut) const;

            bool Request_Receive();

            void Request_Send();

            void Response_Receive();

            void Response_Reply();

        private:

            NO_COPY(Transaction);

            void File_Receive(unsigned int aSize_byte);

            unsigned int Header_Receive(unsigned int* aSize_byte);

            bool Request_Parse();

            void Response_Parse(unsigned int aSize_byte);

            void RetrieveTime(char* aOut, unsigned int aOutSize_byte);

            char mBuffer[8192];

            File::Binary* mFile;
            bool          mFile_Delete;

            unsigned int mMajor;
            unsigned int mMinor;

            std::string mPath;

            Net::Socket* mSocket;
            bool         mSocket_Delete;

            Type mType;

            // ===== Response ===============================================

            const void * mData;
            unsigned int mDataSize_byte;

            Result mResult;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Transaction& aR);

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Transaction::Type& aT);
