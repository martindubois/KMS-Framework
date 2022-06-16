
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/File/Folder.h>
#include <KMS/Message/IReceiver.h>

namespace KMS
{
    namespace HTTP
    {
        class Request;

        class FileServer : public Config::Configurable, public Message::IReceiver
        {

        public:

            typedef void (*FileTypeFunction)(Request*);

            static const unsigned int CODE_ON_REQUEST;

            static void FileType_App_JS    (Request* aRequest);
            static void FileType_Text_CSS  (Request* aRequest);
            static void FileType_Text_HTML (Request* aRequest);
            static void FileType_Text_Plain(Request* aRequest);

            static int Main(int aCount, const char** aVector);

            FileServer();

            void RemoveFileType(const char* aExt);

            void SetFileType(const char* aExt, FileTypeFunction aFunction);

            void SetRoot(const File::Folder& aR);

            void SetVerbose(bool aV = true);

            void ProcessRequest(Request* aR);

            void ProcessRequest_GET(Request* aR);

            // ===== Config::Configurable ===================================
            virtual bool SetAttribute(const char* aA, const char* aV);

            // ===== Message::IReceived =====================================
            bool Receive(void* aSender, unsigned int aCode, void* aData);

        private:

            typedef std::map<std::string, FileTypeFunction> FileTypeMap;

            bool OnRequest(void* aData);

            FileTypeMap mFileTypes;

            // ===== Configurable attributes ================================
            File::Folder mRoot;
            bool mVerbose;

        };

    }
}
