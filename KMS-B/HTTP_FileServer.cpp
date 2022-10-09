
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_FileServer.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Windows ============================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt32.h>
#include <KMS/HTTP/Request.h>
#include <KMS/HTTP/Server.h>

#include <KMS/HTTP/FileServer.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-FileServer.cfg")

#define DEFAULT_ROOT (".")

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_ON_REQUEST (1)

static const KMS::Cfg::MetaData MD_ROOT   ("Root = {Path}");
static const KMS::Cfg::MetaData MD_VERBOSE("Verbose = false | true");

#define NAME_CONTENT_DISPOSITION "content-disposition"

static const KMS::DI::String INLINE("inline");

#define NAME_CONTENT_LENGTH "content-length"

#define NAME_CONTENT_TYPE "content-type"

static const KMS::DI::String APPLICATION_JAVASCRIPT("application/javascript");
static const KMS::DI::String IMAGE_X_ICON          ("image/x-icon");
static const KMS::DI::String TEXT_CSS              ("text/css");
static const KMS::DI::String TEXT_HTML             ("text/html; charset=utf-8");
static const KMS::DI::String TEXT_PLAIN            ("text/plain; charset=utf-8");



namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void FileServer::FileType_App_JS(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.AddEntry(NAME_CONTENT_TYPE, &APPLICATION_JAVASCRIPT);
        }

        void FileServer::FileType_Image_XIcon(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.AddEntry(NAME_CONTENT_TYPE, &IMAGE_X_ICON);
        }

        void FileServer::FileType_Text_CSS(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.AddEntry(NAME_CONTENT_TYPE, &TEXT_CSS);
        }

        void FileServer::FileType_Text_HTML(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.AddEntry(NAME_CONTENT_TYPE, &TEXT_HTML);
        }

        void FileServer::FileType_Text_Plain(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader.AddEntry(NAME_CONTENT_TYPE, &INLINE);
            aRequest->mResponseHeader.AddEntry(NAME_CONTENT_TYPE, &TEXT_PLAIN);
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

                lS.mOnRequest = lFS.ON_REQUEST;

                lC.AddConfigurable(&lFS);
                lC.AddConfigurable(&lS.mSocket);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                Dbg::gLog.CloseLogFiles();

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

        FileServer::FileServer()
            : ON_REQUEST(this, MSG_ON_REQUEST)
            , mRoot   (DEFAULT_ROOT)
            , mVerbose(false)
        {
            AddEntry("Root"   , &mRoot   , false, &MD_ROOT);
            AddEntry("Verbose", &mVerbose, false, &MD_VERBOSE);

            SetFileType("css" , FileType_Text_CSS);
            SetFileType("htm" , FileType_Text_HTML);
            SetFileType("html", FileType_Text_HTML);
            SetFileType("ico" , FileType_Image_XIcon);
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

            if (!mRoot.Get().DoesFileExist(lPath + 1))
            {
                aR->SetResult(Request::Result::NOT_FOUND);
                return;
            }

            lIt->second(aR);

            File::Binary* lFile = new File::Binary(mRoot, lPath + 1);
            assert(NULL != lFile);

            DI::UInt32* lValue = new DI::UInt32(lFile->GetSize());
            aR->mResponseHeader.AddEntry(NAME_CONTENT_LENGTH, lValue);

            aR->SetFile(lFile);
        }

        // ===== Msg::IReceiver =============================================

        unsigned int FileServer::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult;

            switch (aCode)
            {
            case MSG_ON_REQUEST: lResult = OnRequest(aData); break;

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
