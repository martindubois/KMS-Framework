
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h
// Status
// Library   KMS-B

#pragma once

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/HTTP/FileServer.h>
#include <KMS/HTTP/Server.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace HTTP
    {

        class ReactApp : public DI::Dictionary
        {

        public:

            ReactApp();

            void AddFunction(const char* aPath, const ICallback* aCallback);
            void AddRoute   (const char* aPath);

            Server mServer;

        private:

            typedef std::map<std::string, ICallback*> FunctionMap;

            NO_COPY(ReactApp);

            void LocateFrontEnd();

            unsigned int OnFunction(Request* aRequest);

            // ===== Callbacks ==============================================
            const Callback<ReactApp> ON_REQUEST;
            unsigned int OnRequest(void* aSender, void* aData);

            FileServer      mFileServer;
            FunctionMap     mFunctions;
            StringSet_ASCII mRoutes;

        };

    }
}
