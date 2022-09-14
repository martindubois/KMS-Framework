
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Log.h

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurable.h>
#include <KMS/Dbg/LogFile.h>
#include <KMS/File/Folder.h>
#include <KMS/Version.h>

namespace KMS
{
    namespace Dbg
    {

        class Log : public Cfg::Configurable
        {

        public:

            Log();

            ~Log();

            bool IsFileEnabled() const;

            void ResetConsoleLevel();

            void ResetFileLevel();

            void ResetFolder();

            void SetConsoleLevel(LogFile::Level aL);

            void SetFileLevel(LogFile::Level aL);

            void SetFolder(const File::Folder& aF);

            void WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, LogFile::Level aLevel);

            void WriteMessage(const char* aMsg);

            void WriteData(const void* aData, unsigned int aSize_byte);

            void WriteException(const Exception& aException);

            void WriteVersion(const char* aProduct, const Version& aVersion);

            // ===== Cfg::Configurable ======================================
            virtual void DisplayHelp(FILE* aOut) const;
            virtual bool SetAttribute(const char* aN, const char* aV);

        private:

            typedef std::map<unsigned int, LogFile*> FileMap;

            Log(const Log&);

            const Log& operator = (const Log&);

            void CloseLogFiles();

            LogFile* FindLogFile();

            unsigned int   mCounter;
            bool           mEnabled;
            LogFile::Level mEntryLevel;
            FileMap        mFiles;
            unsigned int   mProcessId;

            // ===== Configurable attributes ================================
            LogFile::Level mConsoleLevel;
            LogFile::Level mFileLevel;
            File::Folder   mFolder;

        };

        extern Log gLog;

    }
}

// Macros
// //////////////////////////////////////////////////////////////////////////

#define KMS_DBG_LOG_ERROR()   KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_ERROR);
#define KMS_DBG_LOG_WARNING() KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_WARNING);
#define KMS_DBG_LOG_INFO()    KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_INFO);
#define KMS_DBG_LOG_DEBUG()   KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_DEBUG);
