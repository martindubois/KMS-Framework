
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_FileServer.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/Config/Configurator.h>
#include <KMS/Convert.h>
#include <KMS/HTTP/Request.h>
#include <KMS/HTTP/Server.h>

#include <KMS/HTTP/FileServer.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-FileServer.cfg")

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int FileServer::CODE_ON_REQUEST = 1;

        int FileServer::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);
            assert(NULL != aVector[0]);

            int lResult = __LINE__;

            Network::Thread_Startup();

            try
            {
                KMS::Config::Configurator lC;
                KMS::HTTP::FileServer     lFS;
                KMS::HTTP::Server         lS;

                lS.mOnRequest.Set(&lFS, FileServer::CODE_ON_REQUEST);

                lFS.InitConfigurator(&lC);
                lS.mSocket.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE, false);
                lC.ParseFile(File::Folder(File::Folder::Id::HOME      ), CONFIG_FILE, false);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE, false);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lS.mThread.Start();

                std::cout << "Press ENTER to stop the file server" << std::endl;

                char lLine[1024];

                fgets(lLine, sizeof(lLine), stdin);

                lS.mThread.StopAndWait(2000);

                lResult = 0;
            }
            KMS_CATCH_RESULT(lResult);

            Network::Thread_Cleanup();

            return lResult;
        }

        FileServer::FileServer() : mRoot("."), mVerbose(false) { }

        void FileServer::SetRoot(const File::Folder& aR) { mRoot = aR; }

        void FileServer::SetVerbose(bool aV) { mVerbose = aV; }

        void FileServer::ProcessRequest(Request* aR)
        {
            assert(NULL != aR);

            switch (aR->GetType())
            {
            case Request::Type::GET: ProcessRequest_GET(aR); break;

            default: aR->SetResult(Request::Result::METHOD_NOT_ALLOWED);
            }

            if (mVerbose)
            {
                std::cout << *aR;
            }
        }

        void FileServer::ProcessRequest_GET(Request* aR)
        {
            assert(NULL != aR);

            if (mRoot.DoesFileExist(aR->GetPath()))
            {
                File::Binary* lFile = new File::Binary(mRoot, aR->GetPath() + 1);
                assert(NULL != lFile);

                // TODO Use a map to retrieve the Content-Type and Content-Disposition from the file extension.

                aR->mResponseHeader.Set("Content-Disposition", "inline");
                aR->mResponseHeader.Set("Content-Length", lFile->GetSize());
                aR->mResponseHeader.Set("Content-Type", "text/plain; charset=utf-8");

                aR->SetFile(lFile);
            }
            else
            {
                aR->SetResult(Request::Result::NOT_FOUND);
            }
        }

        // ===== Config::Configurable =======================================

        bool FileServer::SetAttribute(const char* aA)
        {
            assert(NULL != aA);

            if (0 == strcmp(aA, "Verbose")) { SetVerbose(); return true; }

            return Configurable::SetAttribute(aA);
        }

        bool FileServer::SetAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            char lE[1024];

            if (0 == strcmp(aA, "Root")) { Expand(aV, lE, sizeof(lE)); SetRoot(File::Folder(lE)); return true; }
            if (0 == strcmp(aA, "Verbose")) { SetVerbose(Convert::ToBool(aV)); return true; }

            return Configurable::SetAttribute(aA, aV);
        }

        // ===== Message::IReceiver =========================================

        bool FileServer::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            bool lResult = false;

            switch (aCode)
            {
            case CODE_ON_REQUEST: lResult = OnRequest(aData); break;

            default: assert(false);
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool FileServer::OnRequest(void* aData)
        {
            bool lResult = false;

            try
            {
                ProcessRequest(reinterpret_cast<Request*>(aData));
                lResult = true;
            }
            KMS_CATCH;

            return lResult;
        }

    }
}
