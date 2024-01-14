
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Client.h
// Status
// Library   KMS-B

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Binary.h>
#include <KMS/Net/Socket_Client.h>

namespace KMS
{
    namespace HTTP
    {

        class Client final
        {

        public:

            Client(bool aSecure);

            ~Client();

            const DI::Object* GetResponseData() const;

            void Get(const char* aURL, File::Binary* aOutFile = nullptr);

            void Post(const char* aURL, const DI::Object* aData);

        private:

            void ConnectSocket(const char* aHost);

            void ParseURL(const char* aURL, char* aHost, unsigned int aHostSize_byte, char* aPath, unsigned int aPathSize_byte);

            void ReceiveRestOfFile(unsigned int aToRecv_byte, File::Binary* aOutFile);

            void Response_DeleteData();
            void Response_SetData(const DI::Object* aData);

            const DI::Object* mResponse_Data;

            bool mSecure;

            Net::Socket_Client* mSocket;

        };

    }
}
