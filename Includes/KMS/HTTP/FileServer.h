
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

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Folder.h>

namespace KMS
{
    namespace HTTP
    {
        class Transaction;

        /// \brief TODO Remove use of DI namespace
        class FileServer final : public DI::Dictionary
        {

        public:

            static const char* ROOT_DEFAULT;
            static const bool  VERBOSE_DEFAULT;

            DI::Folder  mRoot;
            DI::Boolean mVerbose;

            typedef void (*FileTypeFunction)(Transaction*);

            static void FileType_App_JS     (Transaction* aRequest);
            static void FileType_Image_XIcon(Transaction* aRequest);
            static void FileType_Text_CSS   (Transaction* aRequest);
            static void FileType_Text_HTML  (Transaction* aRequest);
            static void FileType_Text_Plain (Transaction* aRequest);

            static int Main(int aCount, const char** aVector);

            const Callback<FileServer> ON_REQUEST;

            FileServer();

            void RemoveFileType(const char* aExt);

            void SetFileType(const char* aExt, FileTypeFunction aFunction);

            void SetRoot(const File::Folder& aR);

            void SetVerbose(bool aV = true);

            void ProcessRequest(Transaction* aR, const char* aPath = nullptr);

            void ProcessRequest_GET(Transaction* aR, const char* aPath = nullptr);

            // ===== DI::Container ==========================================
            virtual void Validate() const;

        private:

            typedef std::map<String_ASCII, FileTypeFunction> FileTypeMap;

            // ===== Callbacks ==============================================
            unsigned int OnRequest(void* aSender, void* aData);

            FileTypeMap mFileTypes;

        };

    }
}
