
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Request.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/File/Binary.h>
#include <KMS/File/Folder.h>
#include <KMS/JSON/Dictionary.h>
#include <KMS/Net/Socket.h>

namespace KMS
{
    namespace HTTP
    {

        class Request
        {

        public:

            enum class Result
            {
                OK                    = 200,
                CREATED               = 201,
                ACCEPTED              = 202,
                NO_CONTENT            = 204,
                RESET_CONTENT         = 205,
                BAD_REQUEST           = 400,
                FORBIDDEN             = 403,
                NOT_FOUND             = 404,
                METHOD_NOT_ALLOWED    = 405,
                NOT_ACCEPTABLE        = 406,
                REQUEST_TIMEOUT       = 408,
                INTERNAL_SERVER_ERROR = 500,
            };

            enum class Type
            {
                GET,
                OPTIONS,
                POST,
            };

            void AddData(const JSON::Dictionary& aD);
            void AddData(const File::Folder& aF, const char* aFile);

            const JSON::Dictionary& GetParameters() const;

            const char* GetPath() const;

            const JSON::Dictionary& GetRequestHeader() const;

            Type GetType() const;

            const char* GetTypeName() const;

            void SetData(const void* aData, unsigned int aDataSize_byte);

            void SetFile(File::Binary* aF);

            void SetResult(Result aR);

            JSON::Dictionary mResponseData;
            JSON::Dictionary mResponseHeader;

        // Internal

            Request(Net::Socket* aSocket);

            ~Request();

            const char* GetResultName() const;

            bool Receive();

            void Reply();

        private:

            Request(const Request&);

            const Request& operator = (const Request&);

            bool Parse();

            char mBuffer[1024];

            const void * mData;
            unsigned int mDataSize_byte;

            File::Binary* mFile;

            JSON::Dictionary mRequestHeader;

            unsigned int mMajor;
            unsigned int mMinor;

            std::string mPath;

            Result mResult;

            Net::Socket* mSocket;

            Type mType;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Request& aR);

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Request::Result& aR);

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Request::Type& aT);
