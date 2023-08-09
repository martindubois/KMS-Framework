
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

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Folder.h>

namespace KMS
{
    namespace HTTP
    {
        class Request;

        class FileServer : public DI::Dictionary
        {

        public:

            typedef void (*FileTypeFunction)(Request*);

            static void FileType_App_JS     (Request* aRequest);
            static void FileType_Image_XIcon(Request* aRequest);
            static void FileType_Text_CSS   (Request* aRequest);
            static void FileType_Text_HTML  (Request* aRequest);
            static void FileType_Text_Plain (Request* aRequest);

            static int Main(int aCount, const char** aVector);

            const Callback<FileServer> ON_REQUEST;

            FileServer();

            void RemoveFileType(const char* aExt);

            void SetFileType(const char* aExt, FileTypeFunction aFunction);

            void SetRoot(const File::Folder& aR);

            void SetVerbose(bool aV = true);

            void ProcessRequest(Request* aR, const char* aPath = nullptr);

            void ProcessRequest_GET(Request* aR, const char* aPath = nullptr);

            // ===== Configurable attributes ================================
            DI::Folder  mRoot;
            DI::Boolean mVerbose;

        private:

            typedef std::map<std::string, FileTypeFunction> FileTypeMap;

            // ===== Callbacks ==============================================
            unsigned int OnRequest(void* aSender, void* aData);

            FileTypeMap mFileTypes;

        };

    }
}
