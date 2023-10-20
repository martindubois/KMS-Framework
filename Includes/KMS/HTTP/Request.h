
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Request.h
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

        class Request final
        {

        public:

            enum class Type
            {
                GET,
                OPTIONS,
                POST,
            };

            Request();

            void AddData(const DI::Dictionary& aD);
            void AddData(const File::Folder& aF, const char* aFile);

            const DI::Dictionary& GetParameters() const;

            const char* GetPath() const;

            const DI::Dictionary& GetRequestHeader() const;

            Type GetType() const;

            const char* GetTypeName() const;

            void SetData(const void* aData, unsigned int aDataSize_byte);

            void SetFile(File::Binary* aF);

            void SetResult(Result aR);

            DI::Dictionary mResponseData;
            DI::Dictionary mResponseHeader;

        // Internal

            Request(Net::Socket* aSocket);

            ~Request();

            const char* GetResultName() const;

            void Display(std::ostream& aOut) const;

            bool Receive();

            void Reply();

        private:

            NO_COPY(Request);

            void Construct();

            bool Parse();

            char mBuffer[1024];

            const void * mData;
            unsigned int mDataSize_byte;

            File::Binary* mFile;

            DI::Dictionary mRequestHeader;

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

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Request::Type& aT);
