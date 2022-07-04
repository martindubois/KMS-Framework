
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h

#pragma once

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/HTTP/FileServer.h>
#include <KMS/HTTP/Server.h>
#include <KMS/Message/Sender.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace HTTP
    {

        class ReactApp : public Config::Configurable, public Message::IReceiver
        {

        public:

            ReactApp();

            void AddFunction(const char* aRelPath, Message::IReceiver* aReceiver, unsigned int aCode);
            void AddRoute   (const char* aRelPath);

            // ===== Config::Configurable ===================================
            virtual void InitConfigurator(Config::Configurator* aC);

            // ===== Message::IReceiver =====================================
            virtual bool Receive(void* aSender, unsigned int aCode, void* aData);

            Server mServer;

        private:

            typedef std::map<std::string, Message::Sender> FunctionMap;

            ReactApp(const ReactApp&);

            const ReactApp& operator = (const ReactApp&);

            void LocateFrontEnd();

            bool OnFunction(Request* aRequest);
            bool OnRequest (void   * aData   );

            FileServer  mFileServer;
            FunctionMap mFunctions;
            StringSet   mRoutes;

        };

    }
}
