
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log.cpp

// TEST COVERAGE 2023-08-06 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/OS.h>
#include <KMS/Proc/Process.h>

#include <KMS/Dbg/Log.h>

KMS_RESULT_STATIC(RESULT_UNEXPECTED_EXCEPTION);

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Log::CONSOLE_MODE_NAMES[] = { "DEBUG", "USER" };

        const LogFile::Level   Log::CONSOLE_LEVEL_DEFAULT = LogFile::Level::LEVEL_WARNING;
        const Log::ConsoleMode Log::CONSOLE_MODE_DEFAULT  = Log::ConsoleMode::MODE_USER;
        const LogFile::Level   Log::FILE_LEVEL_DEFAULT    = LogFile::Level::LEVEL_INFO;

        const unsigned int Log::FLAG_USER_REDUNDANT = 0x00000001;

        Log::Log()
            : mConsoleLevel(CONSOLE_LEVEL_DEFAULT)
            , mConsoleMode (CONSOLE_MODE_DEFAULT)
            , mFileLevel   (FILE_LEVEL_DEFAULT)
            , mFolder      (File::Folder(File::Folder::HOME, "KMS-Framework"))
            , mCounter(0)
            , mEntryLevel(LogFile::Level::LEVEL_NOISE)
            , mProcessId(OS::GetProcessId())
        {
            CloseLogFiles();
        }

        Log::~Log() { CloseLogFiles(); }

        bool Log::IsFileEnabled() const { return mEnabled; }

        void Log::CloseLogFiles()
        {
            for (const auto& lVT : mFiles)
            {
                delete lVT.second;
            }

            mFiles.clear();

            mEnabled = mFolder.DoesExist();
        }

        #define IF_FILE       if (mEnabled && (mFileLevel >= mEntryLevel))
        #define IF_CONSOLE(F) if ((0 == ((F) & FLAG_USER_REDUNDANT)) && (mConsoleLevel >= mEntryLevel))

        void Log::WriteData(const void* aData, unsigned int aSize_byte, unsigned int aFlags)
        {
            IF_FILE
            {
                auto lLF = FindLogFile();
                if (nullptr != lLF)
                {
                    lLF->WriteData(aData, aSize_byte);
                }
            }

            IF_CONSOLE(aFlags)
            {
                switch (mConsoleMode)
                {
                case ConsoleMode::MODE_DEBUG:
                    switch (mEntryLevel)
                    {
                    case LogFile::Level::LEVEL_ERROR:
                        std::cerr << Console::Color::RED;
                        std::cerr << "Data\t" << aSize_byte << " bytes";
                        std::cerr << Console::Color::WHITE << std::endl;
                        break;

                    case LogFile::Level::LEVEL_WARNING:
                        std::cerr << Console::Color::YELLOW;
                        std::cerr << "Data\t" << aSize_byte << " bytes";
                        std::cerr << Console::Color::WHITE << std::endl;
                        break;

                    case LogFile::Level::LEVEL_INFO:
                    case LogFile::Level::LEVEL_NOISE:
                        // NOT TESTED
                        std::cerr << "Data\t" << aSize_byte << "bytes";
                        std::cerr << std::endl;
                        break;

                    default: assert(false);
                    }
                }
            }
        }

        void Log::WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, Dbg::LogFile::Level aLevel, unsigned int aFlags)
        {
            mEntryLevel = aLevel;

            mCounter++;

            IF_FILE
            {
                auto lLF = FindLogFile();
                if (nullptr != lLF)
                {
                    lLF->WriteEntry(mCounter, aFile, aFunction, aLine, aLevel);
                }
            }

            IF_CONSOLE(aFlags)
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << Console::Color::RED;
                    DisplayInConsole("ERROR", aFile, aFunction, aLine);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case LogFile::Level::LEVEL_WARNING:
                    std::cerr << Console::Color::YELLOW;
                    DisplayInConsole("WARNING", aFile, aFunction, aLine);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case LogFile::Level::LEVEL_INFO:
                    // NOT TESTED
                    DisplayInConsole("INFO", aFile, aFunction, aLine);
                    break;

                case LogFile::Level::LEVEL_NOISE:
                    // NOT TESTED
                    DisplayInConsole("NOISE", aFile, aFunction, aLine);
                    break;

                default: assert(false);
                }
            }
        }

        void Log::WriteException(const Exception& aException, unsigned int aFlags)
        {
            IF_FILE
            {
                auto lLF = FindLogFile();
                if (nullptr != lLF)
                {
                    lLF->WriteException(aException);
                }
            }

            IF_CONSOLE(aFlags)
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << Console::Color::RED;
                    DisplayInConsole(aException);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case LogFile::Level::LEVEL_WARNING:
                    std::cerr << Console::Color::YELLOW;
                    DisplayInConsole(aException);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case LogFile::Level::LEVEL_INFO:
                case LogFile::Level::LEVEL_NOISE:
                    // NOT TESTED
                    DisplayInConsole(aException);
                    break;

                default: assert(false);
                }
            }
        }

        void Log::WriteMessage(const char* aMsg, unsigned int aFlags)
        {
            IF_FILE
            {
                auto lLF = FindLogFile();
                if (nullptr != lLF)
                {
                    lLF->WriteMessage(aMsg);
                }
            }

            IF_CONSOLE(aFlags)
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << Console::Color::RED;
                    DisplayInConsole(aMsg);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case LogFile::Level::LEVEL_WARNING:
                    std::cerr << Console::Color::YELLOW;
                    DisplayInConsole(aMsg);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case LogFile::Level::LEVEL_INFO:
                case LogFile::Level::LEVEL_NOISE:
                    // NOT TESTED
                    DisplayInConsole(aMsg);
                    break;

                default: assert(false);
                }
            }
        }

        Log gLog;

        // Private
        // //////////////////////////////////////////////////////////////////

        void Log::DisplayInConsole(const char* aMsg)
        {
            assert(nullptr != aMsg);

            switch (mConsoleMode)
            {
            case ConsoleMode::MODE_DEBUG:
                std::cerr << "Message\t\"" << aMsg << "\"" << std::endl;
                break;

            case ConsoleMode::MODE_USER:
                std::cerr << aMsg << std::endl;
                break;

            default: assert(false);
            }
        }

        void Log::DisplayInConsole(const char* aTitle, const char* aFile, const char* aFunction, unsigned int aLine)
        {
            assert(nullptr != aTitle);
            assert(nullptr != aFile);
            assert(0 != aLine);

            switch (mConsoleMode)
            {
            case ConsoleMode::MODE_DEBUG:
                std::cerr << aTitle << "\n";
                std::cerr << "    Counter  : " << mCounter << "\n";
                std::cerr << "    File     : " << aFile << " (" << aLine << ")\n";
                std::cerr << "    Function : " << aFunction << std::endl;
                break;

            case ConsoleMode::MODE_USER:
                std::cerr << aTitle << "  " << aFile << " (" << aLine << ")" << std::endl;
                break;

            default: assert(false);
            }
        }

        void Log::DisplayInConsole(const Exception& aException)
        {
            switch (mConsoleMode)
            {
            case ConsoleMode::MODE_DEBUG:
                std::cerr << "Exception\n";
                std::cerr << aException << std::endl;
                break;

            case ConsoleMode::MODE_USER:
                std::cerr << "Exception  " << aException.what() << std::endl;
                break;

            default: assert(false);
            }
        }

        LogFile* Log::FindLogFile()
        {
            LogFile* lResult = nullptr;

            auto lThreadId = OS::GetThreadId();

            auto lIt = mFiles.find(lThreadId);
            if (mFiles.end() == lIt)
            {
                try
                {
                    lResult = new LogFile(mFolder, mProcessId, lThreadId);

                    mFiles.insert(FileMap::value_type(lThreadId, lResult));
                }
                catch (Exception eE)
                {
                    KMS_EXCEPTION_ASSERT(RESULT_OPEN_FAILED == eE.GetCode(), RESULT_UNEXPECTED_EXCEPTION, "Unexpected exception", "");
                }
            }
            else
            {
                assert(nullptr != lIt->second);
                
                lResult = lIt->second;
            }

            return lResult;
        }

    }
}
