
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2022-2026 KMS
/// \file      Includes/KMS/Dbg/Log.h

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/Configurable.h>
#include <KMS/Dbg/LogFile.h>
#include <KMS/Version.h>

namespace KMS
{
    namespace Dbg
    {

        /// \brief How to write log to the console
        enum class ConsoleMode
        {
            MODE_DEBUG,
            MODE_USER,

            QTY
        };

        /// \brief Configuration for the Log
        class Log_Config final
        {

        public:

            KMS_Config_METHODS(Log_Config);

            /// \brief Get the console level
            /// \retval Level::ERROR
            /// \retval Level::INFO
            /// \retval Level::NOISE
            /// \retval Level::WARNING
            /// \see SetConsoleLevel
            Level GetConsoleLevel() const;

            /// \brief Get the console mode
            /// \retval ConsoleMode::DEBUG
            /// \retval ConsoleMode::USER
            /// \see SetConsoleMode
            ConsoleMode GetConsoleMode() const;

            /// \brief Get the file level
            /// \retval Level::ERROR
            /// \retval Level::INFO
            /// \retval Level::NOISE
            /// \retval Level::WARNING
            /// \see SetFileLevel
            Level GetFileLevel() const;

            /// \brief Get the log folder name
            /// \return This method returns the address of an internal buffer
            /// \see SetFolder
            const char* GetFolder() const;

            /// \brief Change the console level
            /// \param aLevel Level::LEVEL_...
            /// \see GetConsoleLevel
            void SetConsoleLevel(Level aLevel);

            /// \brief Change the console mode
            /// \param aMode ConsoleMode::MODE_...
            /// \see GetConsoleMode
            void SetConsoleMode(ConsoleMode aMode);

            /// \brief Change the file level
            /// \param aLevel Level::LEVEL_...
            /// \see GetFileLevel
            void SetFileLevel(Level aLevel);

            /// \brief Change the log folder
            /// \param aFolder The new folder
            /// \see GetFolder
            void SetFolder(const char* aFolder);

        // Internal

            Level       mConsoleLevel;
            ConsoleMode mConsoleMode;
            Level       mFileLevel;
            char        mFolder[PATH_LENGTH];

        };

        /// \brief Log
        class Log final : public Configurable<Log_Config>
        {

        public:

            /// \brief This flag indicate this information is redundant for
            ///        the user
            static const unsigned int FLAG_USER_REDUNDANT;

            /// \brief Consuctor
            Log();

            /// \brief Destructor
            ~Log();

            /// \brief Is logging to a file enabled?
            /// \retval false
            /// \retval true
            bool IsFileEnabled() const;

            void CloseLogFiles();

            /// \brief Write data
            /// \param aData      The data to write
            /// \param aSize_byte The data size
            /// \param aFlags
            /// \see FLAG_USER_REDUNDANT WriteEntry
            void WriteData(const void* aData, unsigned int aSize_byte, unsigned int aFlags = 0);

            /// \brief Write an entry
            /// \param aFile     The source file name
            /// \param aFunction The source function name
            /// \param aLine     The source line
            /// \param aLevel
            /// \param aFlag
            /// \see FLAG_USER_REDUNDANT WriteData WriteException WriteMessage
            void WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, Level aLevel, unsigned int aFlags = 0);

            /// \brief Write an exception
            /// \param aException
            /// \param aFlag
            /// \see FLAG_USER_REDUNDANT WriteEntry
            void WriteException(const Exception& aException, unsigned int aFlags = 0);

            /// \brief Write a message
            /// \param aMsg
            /// \param aFlag
            /// \see FLAG_USER_REDUNDANT WriteEntry
            void WriteMessage(const char* aMsg, unsigned int aFlags = 0);

            // ===== Configurable ===========================================
            virtual void Config_Set(const Log_Config& aIn);

        private:

            typedef std::map<unsigned int, LogFile*> FileMap;

            NO_COPY(Log);

            void DisplayInConsole(const char* aMessage);
            void DisplayInConsole(const char* aTitle, const char* aFile, const char* aFunction, unsigned int aLine);
            void DisplayInConsole(const Exception& aException);

            LogFile* FindLogFile();

            void VerifyFolder();

            unsigned int   mCounter;
            Level          mEntryLevel;
            bool           mFileEnabled;
            FileMap        mFiles;
            unsigned int   mProcessId;

        };

        extern const char* ConsoleMode_NAMES[];

        /// \brief The global log instance
        extern Log gLog;

        // Public
        // //////////////////////////////////////////////////////////////////

        inline Level       Log_Config::GetConsoleLevel() const { return mConsoleLevel; }
        inline ConsoleMode Log_Config::GetConsoleMode () const { return mConsoleMode; }
        inline Level       Log_Config::GetFileLevel   () const { return mFileLevel; }
        inline const char* Log_Config::GetFolder      () const { return mFolder; }

        inline void Log_Config::SetConsoleLevel(Level       aLevel) { mConsoleLevel = aLevel; }
        inline void Log_Config::SetConsoleMode (ConsoleMode aMode ) { mConsoleMode = aMode; }
        inline void Log_Config::SetFileLevel   (Level       aLevel) { mFileLevel = aLevel; }

    }
}

// Macros
// //////////////////////////////////////////////////////////////////////////

#define KMS_DBG_LOG_ERROR()      KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::Level::LEVEL_ERROR);
#define KMS_DBG_LOG_ERROR_F(F)   KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::Level::LEVEL_ERROR, (F));
#define KMS_DBG_LOG_WARNING()    KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::Level::LEVEL_WARNING);
#define KMS_DBG_LOG_WARNING_F(F) KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::Level::LEVEL_WARNING, (F));
#define KMS_DBG_LOG_INFO()       KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::Level::LEVEL_INFO);
#define KMS_DBG_LOG_INFO_F(F)    KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::Level::LEVEL_INFO, (F));
#define KMS_DBG_LOG_NOISE()      KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::Level::LEVEL_NOISE);
#define KMS_DBG_LOG_NOISE_F(F)   KMS::Dbg::gLog.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::Level::LEVEL_NOISE, (F));
