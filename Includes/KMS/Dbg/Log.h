
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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
#include <KMS/Msg/IReceiver.h>
#include <KMS/Version.h>

namespace KMS
{
    namespace Dbg
    {

        class Log : public DI::Dictionary, public Msg::IReceiver
        {

        public:

            enum class ConsoleMode
            {
                DEBUG,
                USER,

                QTY
            };

            static const char* CONSOLE_MODE_NAMES[];

            static const unsigned int FLAG_USER_REDUNDANT;

            Log();

            ~Log();

            bool IsFileEnabled() const;

            void SetHideCount(LogFile::Level aLevel, unsigned int aIn);

            void CloseLogFiles();

            void WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, LogFile::Level aLevel, unsigned int aFlags = 0);

            void WriteMessage(const char* aMsg, unsigned int aFlags = 0);

            void WriteData(const void* aData, unsigned int aSize_byte, unsigned int aFlags = 0);

            void WriteException(const Exception& aException, unsigned int aFlags = 0);

            void WriteVersion(const char* aProduct, const Version& aVersion);

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

            // ===== Configurable attributes ================================
            DI::Enum<LogFile::Level, LogFile::LEVEL_NAMES> mConsoleLevel;
            DI::Enum<ConsoleMode   , CONSOLE_MODE_NAMES  > mConsoleMode;
            DI::Enum<LogFile::Level, LogFile::LEVEL_NAMES> mFileLevel;
            DI::Folder                                     mFolder;

        private:

            typedef std::map<unsigned int, LogFile*> FileMap;

            Log(const Log&);

            const Log& operator = (const Log&);

            void DisplayInConsole(const char* aTitle, const char* aFile, const char* aFunction, unsigned int aLine);
            void DisplayInConsole(const Exception& aException);

            LogFile* FindLogFile();

            unsigned int   mCounter;
            bool           mEnabled;
            LogFile::Level mEntryLevel;
            FileMap        mFiles;
            unsigned int   mHideCounts[static_cast<unsigned int>(LogFile::Level::QTY)];
            unsigned int   mProcessId;

        };

        extern Log gLog;

    }
}

// Macros
// //////////////////////////////////////////////////////////////////////////

#define KMS_DBG_LOG_ERROR()      KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_ERROR);
#define KMS_DBG_LOG_ERROR_F(F)   KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_ERROR, (F));
#define KMS_DBG_LOG_WARNING()    KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_WARNING);
#define KMS_DBG_LOG_WARNING_F(F) KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_WARNING, (F));
#define KMS_DBG_LOG_INFO()       KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_INFO);
#define KMS_DBG_LOG_INFO_F(F)    KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_INFO, (F));
#define KMS_DBG_LOG_NOISE()      KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_NOISE);
#define KMS_DBG_LOG_NOISE_F(F)   KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_NOISE, (F));
