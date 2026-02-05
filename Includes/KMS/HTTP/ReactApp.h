
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
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
#include <KMS/DI/Array.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/HTTP/FileServer.h>
#include <KMS/HTTP/Server.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace HTTP
    {

        /// \brief TODO Remove use of DI namespace
        class ReactApp : public DI::Dictionary
        {

        public:

            static const char* DEFAULT_ROUTE;

            DI::Array mRoutes;

            ReactApp();

            void AddFunction(const char* aPath, const ICallback* aCallback);

            void LocateFrontEnd();

            FileServer mFileServer;
            Server     mServer;

        private:

            typedef std::map<String_ASCII, ICallback*> FunctionMap;

            NO_COPY(ReactApp);

            unsigned int OnFunction(Transaction* aTransaction);

            // ===== Callbacks ==============================================
            const Callback<ReactApp> ON_REQUEST;
            unsigned int OnRequest(void* aSender, void* aData);

            FunctionMap mFunctions;

        };

    }
}
