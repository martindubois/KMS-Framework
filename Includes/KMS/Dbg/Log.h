
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Log.h

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/Dbg/LogFile.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Enum.h>
#include <KMS/DI/Folder.h>
#include <KMS/File/Folder.h>
#include <KMS/Version.h>

namespace KMS
{
    namespace Dbg
    {

        class Log : public DI::Dictionary
        {

        public:

            Log();

            ~Log();

            bool IsFileEnabled() const;

            void CloseLogFiles();

            void WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, LogFile::Level aLevel);

            void WriteMessage(const char* aMsg);

            void WriteData(const void* aData, unsigned int aSize_byte);

            void WriteException(const Exception& aException);

            void WriteVersion(const char* aProduct, const Version& aVersion);

            // ===== Configurable attributes ================================
            DI::Enum<LogFile::Level, LogFile::LEVEL_NAMES> mConsoleLevel;
            DI::Enum<LogFile::Level, LogFile::LEVEL_NAMES> mFileLevel;
            DI::Folder                                     mFolder;

        private:

            typedef std::map<unsigned int, LogFile*> FileMap;

            Log(const Log&);

            const Log& operator = (const Log&);

            LogFile* FindLogFile();

            unsigned int   mCounter;
            bool           mEnabled;
            LogFile::Level mEntryLevel;
            FileMap        mFiles;
            unsigned int   mProcessId;

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
