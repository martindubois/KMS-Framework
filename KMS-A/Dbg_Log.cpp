
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log.cpp

// TEST COVERAGE 2022-09-06 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/OS.h>
#include <KMS/Proc/Process.h>

#include <KMS/Dbg/Log.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_CONSOLE_LEVEL (LogFile::Level::LEVEL_WARNING)
#define DEFAULT_CONSOLE_MODE  (Log::ConsoleMode::USER)
#define DEFAULT_FILE_LEVEL    (LogFile::Level::LEVEL_INFO)
#define DEFAULT_FOLDER_NAME   ("KMS-Framework")

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int Log::FLAG_USER_REDUNDANT = 0x00000001;

        Log::Log()
            : mConsoleLevel(DEFAULT_CONSOLE_LEVEL)
            , mConsoleMode (DEFAULT_CONSOLE_MODE)
            , mFileLevel   (DEFAULT_FILE_LEVEL)
            , mFolder      (File::Folder(File::Folder::HOME, DEFAULT_FOLDER_NAME))
            , mCounter(0)
            , mEntryLevel(LogFile::Level::LEVEL_NOISE)
            , mProcessId(OS::GetProcessId())
        {
            memset(&mHideCounts, 0, sizeof(mHideCounts));

            CloseLogFiles();
        }

        Log::~Log() { CloseLogFiles(); }

        bool Log::IsFileEnabled() const { return mEnabled; }

        void Log::SetHideCount(LogFile::Level aLevel, unsigned int aIn)
        {
            WriteEntry(__FILE__, __FUNCTION__, __LINE__, LogFile::Level::LEVEL_INFO);
            WriteData(&aLevel, sizeof(aLevel));
            WriteData(&aIn, sizeof(aIn));

            mHideCounts[static_cast<unsigned int>(aLevel)] = aIn;
        }

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
        #define IF_CONSOLE(F) if ((0 == ((F) & FLAG_USER_REDUNDANT)) && (((!mEnabled) && (LogFile::Level::LEVEL_WARNING >= mEntryLevel)) || (mConsoleLevel >= mEntryLevel)))

        void Log::WriteData(const void* aData, unsigned int aSize_byte, unsigned int aFlags)
        {
            IF_FILE
            {
                auto lLF = FindLogFile();
                assert(NULL != lLF);

                lLF->WriteData(aData, aSize_byte);
            }

            if (0 < mHideCounts[static_cast<unsigned int>(mEntryLevel)])
            {
                mHideCounts[static_cast<unsigned int>(mEntryLevel)]--;
                return;
            }

            IF_CONSOLE(aFlags)
            {
                switch (mConsoleMode)
                {
                case ConsoleMode::DEBUG:
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
                assert(NULL != lLF);

                lLF->WriteEntry(mCounter, aFile, aFunction, aLine, aLevel);
            }

            if (0 < mHideCounts[static_cast<unsigned int>(mEntryLevel)])
            {
                mHideCounts[static_cast<unsigned int>(mEntryLevel)]--;
                return;
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
                assert(NULL != lLF);

                lLF->WriteException(aException);
            }

            if (0 < mHideCounts[static_cast<unsigned int>(mEntryLevel)])
            {
                mHideCounts[static_cast<unsigned int>(mEntryLevel)]--;
                return;
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
                assert(NULL != lLF);

                lLF->WriteMessage(aMsg);
            }

            if (0 < mHideCounts[static_cast<unsigned int>(mEntryLevel)])
            {
                mHideCounts[static_cast<unsigned int>(mEntryLevel)]--;
                return;
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
            assert(NULL != aMsg);

            switch (mConsoleMode)
            {
            case ConsoleMode::DEBUG:
                std::cerr << "Message\t\"" << aMsg << "\"" << std::endl;
                break;

            case ConsoleMode::USER:
                std::cerr << aMsg << std::endl;
                break;

            default: assert(false);
            }
        }

        void Log::DisplayInConsole(const char* aTitle, const char* aFile, const char* aFunction, unsigned int aLine)
        {
            assert(NULL != aTitle);
            assert(NULL != aFile);
            assert(NULL != aLine);

            switch (mConsoleMode)
            {
            case ConsoleMode::DEBUG:
                std::cerr << aTitle << "\n";
                std::cerr << "    Counter  : " << mCounter << "\n";
                std::cerr << "    File     : " << aFile << " (" << aLine << ")\n";
                std::cerr << "    Function : " << aFunction << std::endl;
                break;

            case ConsoleMode::USER:
                std::cerr << aTitle << "  " << aFile << " (" << aLine << ")" << std::endl;
                break;

            default: assert(false);
            }
        }

        void Log::DisplayInConsole(const Exception& aException)
        {
            switch (mConsoleMode)
            {
            case ConsoleMode::DEBUG:
                std::cerr << "Exception\n";
                std::cerr << aException << std::endl;
                break;

            case ConsoleMode::USER:
                std::cerr << "Exception  " << aException.what() << std::endl;
                break;

            default: assert(false);
            }
        }

        LogFile* Log::FindLogFile()
        {
            LogFile* lResult;

            auto lThreadId = OS::GetThreadId();

            auto lIt = mFiles.find(lThreadId);
            if (mFiles.end() == lIt)
            {
                lResult = new LogFile(mFolder, mProcessId, lThreadId);

                mFiles.insert(FileMap::value_type(lThreadId, lResult));
            }
            else
            {
                lResult = lIt->second;
            }

            assert(NULL != lResult);
            return lResult;
        }

    }
}
