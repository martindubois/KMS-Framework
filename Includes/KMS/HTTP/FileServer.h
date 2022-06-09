
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/File/Folder.h>
#include <KMS/Message/IReceiver.h>

namespace KMS
{
    namespace HTTP
    {
        class Request;

        class FileServer : public Config::Configurable, public Message::IReceiver
        {

        public:

            static const unsigned int CODE_ON_REQUEST;

            static int Main(int aCount, const char** aVector);

            FileServer();

            void SetRoot(const File::Folder& aR);
            void SetVerbose(bool aV = true);

            void ProcessRequest(Request* aR);

            void ProcessRequest_GET(Request* aR);

            // ===== Config::Configurable ===================================
            virtual bool SetAttribute(const char* aA);
            virtual bool SetAttribute(const char* aA, const char* aV);

            // ===== Message::IReceived =====================================
            bool Receive(void* aSender, unsigned int aCode, void* aData);

        private:

            bool OnRequest(void* aData);

            // ===== Configurable attributes ================================
            File::Folder mRoot;
            bool mVerbose;

        };

    }
}
