
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
#include <KMS/Cfg/Configurator.h>
#include <KMS/Convert.h>
#include <KMS/Environment.h>
#include <KMS/HTTP/Request.h>
#include <KMS/HTTP/Server.h>

#include <KMS/HTTP/FileServer.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-FileServer.cfg")

#define DEFAULT_ROOT (".")

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int FileServer::CODE_ON_REQUEST = 1;

        void FileServer::FileType_App_JS(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.Set("Content-Type", "application/javascript");
        }

        void FileServer::FileType_Text_CSS(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.Set("Content-Type", "text/css");
        }

        void FileServer::FileType_Text_HTML(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.Set("Content-Type", "text/html; charset=utf-8");
        }

        void FileServer::FileType_Text_Plain(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.Set("Content-Disposition", "inline");
            aRequest->mResponseHeader.Set("Content-Type", "text/plain; charset=utf-8");
        }

        int FileServer::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);
            assert(NULL != aVector[0]);

            int lResult = __LINE__;

            Net::Thread_Startup();

            try
            {
                KMS::Cfg::Configurator lC;
                KMS::HTTP::FileServer  lFS;
                KMS::HTTP::Server      lS;

                lS.mOnRequest.Set(&lFS, FileServer::CODE_ON_REQUEST);

                lFS.InitConfigurator(&lC);
                lS.mSocket.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::HOME      ), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lS.mThread.Start();

                std::cout << "Press ENTER to stop the file server" << std::endl;

                char lLine[LINE_LENGTH];

                fgets(lLine, sizeof(lLine), stdin);

                lS.mThread.StopAndWait(2000);

                lResult = 0;
            }
            KMS_CATCH_RESULT(lResult);

            Net::Thread_Cleanup();

            return lResult;
        }

        FileServer::FileServer() : mRoot(DEFAULT_ROOT), mVerbose(false)
        {
            SetFileType("css" , FileType_Text_CSS);
            SetFileType("htm" , FileType_Text_HTML);
            SetFileType("html", FileType_Text_HTML);
            SetFileType("js"  , FileType_App_JS);
            SetFileType("txt" , FileType_Text_Plain);
        }

        void FileServer::SetFileType(const char* aExt, FileTypeFunction aFunction)
        {
            assert(NULL != aExt);

            FileTypeMap::iterator lIt = mFileTypes.find(aExt);
            if (mFileTypes.end() == lIt)
            {
                mFileTypes.insert(FileTypeMap::value_type(aExt, aFunction));
            }
            else
            {
                lIt->second = aFunction;
            }
        }

        void FileServer::SetRoot(const File::Folder& aR) { mRoot = aR; }

        void FileServer::SetVerbose(bool aV) { mVerbose = aV; }

        void FileServer::ProcessRequest(Request* aR, const char* aPath)
        {
            assert(NULL != aR);

            switch (aR->GetType())
            {
            case Request::Type::GET: ProcessRequest_GET(aR, aPath); break;

            default: aR->SetResult(Request::Result::METHOD_NOT_ALLOWED);
            }

            if (mVerbose)
            {
                std::cout << *aR;
            }
        }

        void FileServer::ProcessRequest_GET(Request* aR, const char* aPath)
        {
            assert(NULL != aR);

            const char* lPath = (NULL == aPath) ? aR->GetPath() : aPath;

            // TODO Protect against .. in path.

            const char* lExt = strrchr(lPath, '.');
            if (NULL == lExt)
            {
                aR->SetResult(Request::Result::FORBIDDEN);
                return;
            }

            FileTypeMap::iterator lIt = mFileTypes.find(lExt + 1);
            if (mFileTypes.end() == lIt)
            {
                aR->SetResult(Request::Result::FORBIDDEN);
                return;
            }

            if (!mRoot.DoesFileExist(lPath + 1))
            {
                aR->SetResult(Request::Result::NOT_FOUND);
                return;
            }

            lIt->second(aR);

            File::Binary* lFile = new File::Binary(mRoot, lPath + 1);
            assert(NULL != lFile);

            aR->mResponseHeader.Set("Content-Length", lFile->GetSize());

            aR->SetFile(lFile);
        }

        // ===== Cfg::Configurable ==========================================

        bool FileServer::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Root") { SetRoot(DEFAULT_ROOT); return true; }

                CFG_IF("Verbose") { SetVerbose(); return true; }
            }
            else
            {
                char lE[LINE_LENGTH];

                CFG_IF("Root") { Env::Expand(aV, lE, sizeof(lE)); SetRoot(File::Folder(lE)); return true; }

                CFG_CONVERT("Verbose", SetVerbose, Convert::ToBool);
            }

            return Configurable::SetAttribute(aA, aV);
        }

        void FileServer::DisplayHelp(FILE* aOut) const
        {
            fprintf(aOut,
                "===== KMS::HTTP::FileServer =====\n"
                "Root\n"
                "    Default: %s\n"
                "Root = {Path}\n"
                "    Set the root\n"
                "Verbose\n"
                "    Set the verbose flag\n"
                "Verbose = {Boolean}\n"
                "    Set or clear the verbose flag\n",
                DEFAULT_ROOT);

            Configurable::DisplayHelp(aOut);
        }

        // ===== Msg::IReceiver =============================================

        unsigned int FileServer::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult;

            switch (aCode)
            {
            case CODE_ON_REQUEST: lResult = OnRequest(aData); break;

            default:
                assert(false);
                lResult = Msg::IReceiver::MSG_IGNORED;
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int FileServer::OnRequest(void* aData)
        {
            ProcessRequest(reinterpret_cast<Request*>(aData));

            return 0;
        }

    }
}
