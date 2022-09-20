
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
#include <KMS/Convert.h>
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt32.h>
#include <KMS/Environment.h>
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

static const KMS::DI::MetaData MD_CONTENT_DISPOSITION("content-disposition", NULL);
static const KMS::DI::MetaData MD_CONTENT_LENGTH     ("content-length"     , NULL, KMS::DI::MetaData::FLAG_DELETE_OBJECT);
static const KMS::DI::MetaData MD_CONTENT_TYPE       ("content-type"       , NULL);

static const KMS::DI::String APPLICATION_JAVASCRIPT("application/javascript"   , &MD_CONTENT_TYPE);
static const KMS::DI::String IMAGE_X_ICON          ("image/x-icon"             , &MD_CONTENT_TYPE);
static const KMS::DI::String TEXT_CSS              ("text/css"                 , &MD_CONTENT_TYPE);
static const KMS::DI::String TEXT_HTML             ("text/html; charset=utf-8" , &MD_CONTENT_TYPE);
static const KMS::DI::String TEXT_PLAIN            ("text/plain; charset=utf-8", &MD_CONTENT_TYPE);

static const KMS::DI::String INLINE("inline", &MD_CONTENT_DISPOSITION);


namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void FileServer::FileType_App_JS(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader += const_cast<DI::String*>(&APPLICATION_JAVASCRIPT);
        }

        void FileServer::FileType_Image_XIcon(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader += const_cast<DI::String*>(&IMAGE_X_ICON);
        }

        void FileServer::FileType_Text_CSS(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader += const_cast<DI::String*>(&TEXT_CSS);
        }

        void FileServer::FileType_Text_HTML(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader += const_cast<DI::String*>(&TEXT_HTML);
        }

        void FileServer::FileType_Text_Plain(Request* aRequest)
        {
            assert(NULL != aRequest);

            aRequest->mResponseHeader += const_cast<DI::String*>(&INLINE);
            aRequest->mResponseHeader += const_cast<DI::String*>(&TEXT_PLAIN);
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

                lFS.InitConfigurator(&lC);
                lS.mSocket.InitConfigurator(&lC);

                Dbg::gLog.InitConfigurator(&lC);

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

        FileServer::FileServer() : ON_REQUEST(this, MSG_ON_REQUEST), mRoot(DEFAULT_ROOT), mVerbose(false)
        {
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

            if (!mRoot.DoesFileExist(lPath + 1))
            {
                aR->SetResult(Request::Result::NOT_FOUND);
                return;
            }

            lIt->second(aR);

            File::Binary* lFile = new File::Binary(mRoot, lPath + 1);
            assert(NULL != lFile);

            DI::UInt32* lValue = new DI::UInt32(lFile->GetSize(), &MD_CONTENT_LENGTH);
            aR->mResponseHeader += lValue;

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
