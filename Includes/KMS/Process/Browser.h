
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Process/Process.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/File/Folder.h>
#include <KMS/HTTP/Server.h>

namespace KMS
{
    namespace Process
    {

        class Process;

        class Browser : public Config::Configurable
        {

        public:

            enum class Type
            {
                CHROME = 0,
                DEFAULT,
                EDGE,
                NONE,

                QTY
            };

            Browser();

            ~Browser();

            bool IsOpen() const;

            // LIMITATION The application mode is not supporter with Edge or
            //            the default browser, even if the default browser is
            //            not Edge.
            void SetAppMode(bool aAM = true);

            // LIMITATION In kiosk mode, the Browser instance cannot close
            //            the Windows or verify if it is still open.
            void SetKioskMode(bool aKM = true);

            void SetPrefered(Type aP);

            void Close();

            void Detach();

            void Open(const File::Folder& aFolder, const char* aFile, const char* aTitle);

            void Open(const HTTP::Server& aServer, const char* aFile, const char* aTitle);

            void Start(const File::Folder& aFolder, const char* aFile);

            // ===== Config::Configurable ===================================
            virtual bool SetAttribute(const char* aA, const char* aV);
            virtual void DisplayHelp(FILE* aOut);

        private:

            bool BuildExpectedWindowsTitle(const char* aTitle, char* aOut, unsigned int aOutSize_byte);

            void Create(const char* aURL);

            void CreateProcess_Chrome (const char* aURL);
            void CreateProcess_Default(const char* aURL);
            void CreateProcess_Edge   (const char* aURL);

            void RetrieveWindow(const char* aTitle);

            void Open(const char* aURL);

            void Open();

            void Start(const char* aURL);

            bool mAppMode;
            bool mKioskMode;

            Process* mProcess;

            Type mType;

            uint64_t mWindow;

            // ===== Configurable attribute =================================
            Type mPrefered;

        };

    }
}
