
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_FileServer.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/HTTP/HTTP.h>
#include <KMS/HTTP/Request.h>
#include <KMS/HTTP/Server.h>
#include <KMS/Main.h>

#include <KMS/HTTP/FileServer.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-FileServer.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_ROOT   ("Root = {Path}");
static const KMS::Cfg::MetaData MD_VERBOSE("Verbose = false | true");

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* FileServer::ROOT_DEFAULT    = ".";
        const bool  FileServer::VERBOSE_DEFAULT = false;

        void FileServer::FileType_App_JS(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(FIELD_NAME_CONTENT_TYPE, &FIELD_VALUE_CONTENT_TYPE_APPLICATION_JAVASCRIPT);
        }

        void FileServer::FileType_Image_XIcon(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(FIELD_NAME_CONTENT_TYPE, &FIELD_VALUE_CONTENT_TYPE_IMAGE_X_ICON);
        }

        void FileServer::FileType_Text_CSS(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(FIELD_NAME_CONTENT_TYPE, &FIELD_VALUE_CONTENT_TYPE_TEXT_CSS);
        }

        void FileServer::FileType_Text_HTML(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(FIELD_NAME_CONTENT_TYPE, &FIELD_VALUE_CONTENT_TYPE_TEXT_HTML);
        }

        void FileServer::FileType_Text_Plain(Request* aRequest)
        {
            assert(nullptr != aRequest);

            aRequest->mResponseHeader.AddConstEntry(FIELD_NAME_CONTENT_DISPOSITION, &FIELD_VALUE_CONTENT_DISPOSITION_INLINE);
            aRequest->mResponseHeader.AddConstEntry(FIELD_NAME_CONTENT_TYPE, &FIELD_VALUE_CONTENT_TYPE_TEXT_PLAIN);
        }

        int FileServer::Main(int aCount, const char** aVector)
        {
            Net::Thread_Startup();

            KMS_MAIN_BEGIN;
            {
                HTTP::FileServer lFS;
                HTTP::Server     lS;

                lS.mOnRequest = &lFS.ON_REQUEST;

                lConfigurator.AddConfigurable(&lFS);
                lConfigurator.AddConfigurable(&lS.mSocket);

                lConfigurator.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lConfigurator.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lConfigurator.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);

                KMS_MAIN_PARSE_ARGS(aCount, aVector);

                KMS_MAIN_VALIDATE;

                lS.mThread.Start();

                std::cout << "Press ENTER to stop the file server" << std::endl;

                char lLine[LINE_LENGTH];

                auto lRet = fgets(lLine, sizeof(lLine), stdin);
                assert(nullptr != lRet);
                (void)lRet;

                lS.mThread.StopAndWait(2000);

                lResult = 0;
            }
            KMS_MAIN_END;

            Net::Thread_Cleanup();

            KMS_MAIN_RETURN;
        }

        FileServer::FileServer()
            : mRoot   (ROOT_DEFAULT)
            , mVerbose(VERBOSE_DEFAULT)
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

            default: aR->SetResult(Result::METHOD_NOT_ALLOWED);
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
                aR->SetResult(Result::FORBIDDEN);
                return;
            }

            auto lIt = mFileTypes.find(lExt + 1);
            if (mFileTypes.end() == lIt)
            {
                aR->SetResult(Result::FORBIDDEN);
                return;
            }

            if (!mRoot.GetFolder().DoesFileExist(lPath + 1))
            {
                aR->SetResult(Result::NOT_FOUND);
                return;
            }

            KMS_DBG_LOG_INFO_F(Dbg::Log::FLAG_USER_REDUNDANT);
            Dbg::gLog.WriteMessage(lPath);

            lIt->second(aR);

            auto lFile = new File::Binary(mRoot, lPath + 1);
            assert(nullptr != lFile);

            auto lValue = new DI::UInt<uint32_t>(lFile->GetSize());
            aR->mResponseHeader.AddEntry(FIELD_NAME_CONTENT_LENGTH, lValue, true);

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
