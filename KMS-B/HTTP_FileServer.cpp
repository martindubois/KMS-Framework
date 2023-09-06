
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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
#include <KMS/Dbg/Log_Cfg.h>
#include <KMS/Dbg/Stats.h>
#include <KMS/Dbg/Stats_Timer.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/HTTP/Request.h>
#include <KMS/HTTP/Server.h>
#include <KMS/Installer.h>

#include <KMS/HTTP/FileServer.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-FileServer.cfg")

#define DEFAULT_ROOT (".")

// Constants
// //////////////////////////////////////////////////////////////////////////

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
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(NAME_CONTENT_TYPE, &APPLICATION_JAVASCRIPT);
        }

        void FileServer::FileType_Image_XIcon(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(NAME_CONTENT_TYPE, &IMAGE_X_ICON);
        }

        void FileServer::FileType_Text_CSS(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(NAME_CONTENT_TYPE, &TEXT_CSS);
        }

        void FileServer::FileType_Text_HTML(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(NAME_CONTENT_TYPE, &TEXT_HTML);
        }

        void FileServer::FileType_Text_Plain(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(NAME_CONTENT_TYPE, &INLINE);
            aRequest->mResponseHeader.AddConstEntry(NAME_CONTENT_TYPE, &TEXT_PLAIN);
        }

        int FileServer::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(nullptr != aVector);
            assert(nullptr != aVector[0]);

            int lResult = __LINE__;

            Net::Thread_Startup();

            auto lET = new Dbg::Stats_Timer("Main_ExecutionTime");
            lET->Start();

            try
            {
                Cfg::Configurator lC;
                HTTP::FileServer  lFS;
                Installer         lInstaller;
                Dbg::Log_Cfg      lLogCfg(&Dbg::gLog);
                HTTP::Server      lS;

                lS.mOnRequest = &lFS.ON_REQUEST;

                lC.AddConfigurable(&lFS);
                lC.AddConfigurable(&lS.mSocket);

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lLogCfg);
                lC.AddConfigurable(&Dbg::gStats);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lC.Validate();

                lInstaller.Run();

                lS.mThread.Start();

                std::cout << "Press ENTER to stop the file server" << std::endl;

                char lLine[LINE_LENGTH];

                fgets(lLine, sizeof(lLine), stdin);

                lS.mThread.StopAndWait(2000);

                lResult = 0;
            }
            KMS_CATCH_RESULT(lResult);

            lET->Stop();

            Net::Thread_Cleanup();

            return lResult;
        }

        FileServer::FileServer()
            : mRoot   (DEFAULT_ROOT)
            , mVerbose(false)
            // ===== Callbacks ==============================================
            , ON_REQUEST(this, &FileServer::OnRequest)
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
            assert(nullptr != aExt);

            auto lIt = mFileTypes.find(aExt);
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
            assert(nullptr != aR);

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
            assert(nullptr != aR);

            auto lPath = (nullptr == aPath) ? aR->GetPath() : aPath;

            // TODO Protect against .. in path.

            auto lExt = strrchr(lPath, '.');
            if (nullptr == lExt)
            {
                aR->SetResult(Request::Result::FORBIDDEN);
                return;
            }

            auto lIt = mFileTypes.find(lExt + 1);
            if (mFileTypes.end() == lIt)
            {
                aR->SetResult(Request::Result::FORBIDDEN);
                return;
            }

            if (!mRoot.GetFolder().DoesFileExist(lPath + 1))
            {
                aR->SetResult(Request::Result::NOT_FOUND);
                return;
            }

            KMS_DBG_LOG_INFO_F(Dbg::Log::FLAG_USER_REDUNDANT);
            Dbg::gLog.WriteMessage(lPath);

            lIt->second(aR);

            auto lFile = new File::Binary(mRoot, lPath + 1);
            assert(nullptr != lFile);

            auto lValue = new DI::UInt<uint32_t>(lFile->GetSize());
            aR->mResponseHeader.AddEntry(NAME_CONTENT_LENGTH, lValue, true);

            aR->SetFile(lFile);
        }

        // ===== DI::Container ==============================================

        void FileServer::Validate() const
        {
            DI::Dictionary::Validate();

            KMS_EXCEPTION_ASSERT(mRoot.GetFolder().DoesExist(), RESULT_INVALID_CONFIG, "The root folder does not exist", mRoot);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Callbacks ==================================================

        unsigned int FileServer::OnRequest(void*, void* aData)
        {
            ProcessRequest(reinterpret_cast<Request*>(aData));

            return 0;
        }

    }
}
