
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h
// Library   KMS-B

#pragma once

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/HTTP/FileServer.h>
#include <KMS/HTTP/Server.h>
#include <KMS/Msg/Destination.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace HTTP
    {

        class ReactApp : public DI::Dictionary, public Msg::IReceiver
        {

        public:

            ReactApp();

            void AddFunction(const char* aPath, Msg::IReceiver* aReceiver, unsigned int aCode);
            void AddRoute   (const char* aPath);

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

            Server mServer;

        private:

            typedef std::map<std::string, Msg::Destination> FunctionMap;

            ReactApp(const ReactApp&);

            const ReactApp& operator = (const ReactApp&);

            void LocateFrontEnd();

            unsigned int OnFunction(Request* aRequest);
            unsigned int OnRequest (void   * aData   );

            FileServer  mFileServer;
            FunctionMap mFunctions;
            StringSet   mRoutes;

        };

    }
}
