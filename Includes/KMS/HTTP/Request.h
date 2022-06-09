
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Request.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/Dictionary.h>
#include <KMS/File/Binary.h>
#include <KMS/File/Folder.h>
#include <KMS/Network/Socket.h>

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

            void AddData(const Dictionary& aD);
            void AddData(const File::Folder& aF, const char* aFile);

            const Dictionary& GetParameters() const;

            const char* GetPath() const;

            const Dictionary& GetRequestHeader() const;

            Type GetType() const;

            const char* GetTypeName() const;

            void SetFile(File::Binary* aF);

            void SetResult(Result aR);

            Dictionary mResponseHeader;

        // Internal

            Request(Network::Socket* aSocket);

            ~Request();

            const char* GetResultName() const;

            bool Receive();

            void Reply();

        private:

            Request(const Request&);

            const Request& operator = (const Request&);

            bool Parse();

            char mBuffer[1024];

            File::Binary* mFile;

            Dictionary mRequestHeader;

            unsigned int mMajor;
            unsigned int mMinor;

            std::string mPath;

            Result mResult;

            Network::Socket* mSocket;

            Type mType;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Request& aR);

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Request::Result& aR);

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Request::Type& aT);
