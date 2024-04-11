
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
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
#include <KMS/HTTP/Server.h>
#include <KMS/HTTP/Transaction.h>
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

        void FileServer::FileType_App_JS(Transaction* aTransaction)
        {
            assert(nullptr != aTransaction);

            Ptr_OF<DI::Object> lEntry(&FIELD_VALUE_CONTENT_TYPE_APPLICATION_JAVASCRIPT);

            aTransaction->mResponse_Header.AddEntry(FIELD_NAME_CONTENT_TYPE, lEntry);
        }

        void FileServer::FileType_Image_XIcon(Transaction* aTransaction)
        {
            assert(nullptr != aTransaction);

            Ptr_OF<DI::Object> lEntry(&FIELD_VALUE_CONTENT_TYPE_IMAGE_X_ICON);

            aTransaction->mResponse_Header.AddEntry(FIELD_NAME_CONTENT_TYPE, lEntry);
        }

        void FileServer::FileType_Text_CSS(Transaction* aTransaction)
        {
            assert(nullptr != aTransaction);

            Ptr_OF<DI::Object> lEntry(&FIELD_VALUE_CONTENT_TYPE_TEXT_CSS);

            aTransaction->mResponse_Header.AddEntry(FIELD_NAME_CONTENT_TYPE, lEntry);
        }

        void FileServer::FileType_Text_HTML(Transaction* aTransaction)
        {
            assert(nullptr != aTransaction);

            Ptr_OF<DI::Object> lEntry(&FIELD_VALUE_CONTENT_TYPE_TEXT_HTML);

            aTransaction->mResponse_Header.AddEntry(FIELD_NAME_CONTENT_TYPE, lEntry);
        }

        void FileServer::FileType_Text_Plain(Transaction* aTransaction)
        {
            assert(nullptr != aTransaction);

            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&Response::FIELD_VALUE_CONTENT_DISPOSITION_INLINE); aTransaction->mResponse_Header.AddEntry(Response::FIELD_NAME_CONTENT_DISPOSITION, lEntry);
            lEntry.Set(&          FIELD_VALUE_CONTENT_TYPE_TEXT_PLAIN   ); aTransaction->mResponse_Header.AddEntry(          FIELD_NAME_CONTENT_TYPE       , lEntry);
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

                lS.Start();

                std::cout << "Press ENTER to stop the file server" << std::endl;

                char lLine[LINE_LENGTH];

                auto lRet = fgets(lLine, sizeof(lLine), stdin);
                assert(nullptr != lRet);
                (void)lRet;

                lS.StopAndWait(2000);

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
            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mRoot   , false); AddEntry("Root"   , lEntry, &MD_ROOT);
            lEntry.Set(&mVerbose, false); AddEntry("Verbose", lEntry, &MD_VERBOSE);

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

        void FileServer::ProcessRequest(Transaction* aT, const char* aPath)
        {
            assert(nullptr != aT);

            switch (aT->GetType())
            {
            case Transaction::Type::GET: ProcessRequest_GET(aT, aPath); break;

            default: aT->SetResult(Result::METHOD_NOT_ALLOWED);
            }

            if (mVerbose)
            {
                std::cout << *aT;
            }
        }

        void FileServer::ProcessRequest_GET(Transaction* aT, const char* aPath)
        {
            assert(nullptr != aT);

            auto lPath = (nullptr == aPath) ? aT->GetPath() : aPath;

            // TODO Protect against .. in path.

            auto lExt = strrchr(lPath, '.');
            if (nullptr == lExt)
            {
                aT->SetResult(Result::FORBIDDEN);
                return;
            }

            auto lIt = mFileTypes.find(lExt + 1);
            if (mFileTypes.end() == lIt)
            {
                aT->SetResult(Result::FORBIDDEN);
                return;
            }

            if (!mRoot.GetFolder().DoesFileExist(lPath + 1))
            {
                aT->SetResult(Result::NOT_FOUND);
                return;
            }

            KMS_DBG_LOG_INFO_F(Dbg::Log::FLAG_USER_REDUNDANT);
            Dbg::gLog.WriteMessage(lPath);

            lIt->second(aT);

            Ptr_OF<File::Binary> lFile(new File::Binary(mRoot, lPath + 1), true);

            Ptr_OF<DI::Object> lEntry(new DI::UInt<uint32_t>(lFile->GetSize()), true);
            aT->mResponse_Header.AddEntry(FIELD_NAME_CONTENT_LENGTH, lEntry);

            aT->SetFile(lFile);
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
            ProcessRequest(reinterpret_cast<Transaction*>(aData));

            return 0;
        }

    }
}
