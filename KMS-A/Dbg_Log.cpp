
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
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

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Log_Config::SetFolder(const char* aFolder)
        {
            assert(nullptr != aFolder);

            strcpy_s(mFolder, aFolder);
        }

        void Log_Config::SetDefault()
        {
            #ifdef _DEBUG
                mConsoleLevel = Level::LEVEL_INFO;
                mConsoleMode = ConsoleMode::MODE_DEBUG;
                mFileLevel = Level::LEVEL_NOISE;
            #else
                mConsoleLevel = Level::LEVEL_WARNING;
                mConsoleMode = ConsoleMode::MODE_USER;
                mFileLevel = Level::LEVEL_INFO;
            #endif

            memset(&mFolder, 0, sizeof(mFolder));
        }

        void Log_Config::Validate() const
        {
            KMS_EXCEPTION_ASSERT(Level::QTY       > mConsoleLevel, RESULT_INVALID_CONFIG, "Invalid console level", "");
            KMS_EXCEPTION_ASSERT(ConsoleMode::QTY > mConsoleMode , RESULT_INVALID_CONFIG, "Invalid console mode" , "");
            KMS_EXCEPTION_ASSERT(Level::QTY       > mFileLevel   , RESULT_INVALID_CONFIG, "Invalid file level"   , "");
        }

        const unsigned int Log::FLAG_USER_REDUNDANT = 0x00000001;

        Log::Log()
            : mCounter(0)
            , mEntryLevel(Level::LEVEL_NOISE)
            , mProcessId(OS::GetProcessId())
        {
            VerifyFolder();
        }

        Log::~Log() { CloseLogFiles(); }

        bool Log::IsFileEnabled() const { return mFileEnabled; }

        void Log::CloseLogFiles()
        {
            for (const auto& lVT : mFiles)
            {
                delete lVT.second;
            }

            mFiles.clear();
        }

        #define IF_FILE       if (mFileEnabled && (mConfig.GetFileLevel() >= mEntryLevel))
        #define IF_CONSOLE(F) if ((0 == ((F) & FLAG_USER_REDUNDANT)) && (mConfig.GetConsoleLevel() >= mEntryLevel))

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
                switch (mConfig.GetConsoleMode())
                {
                case ConsoleMode::MODE_DEBUG:
                    switch (mEntryLevel)
                    {
                    case Level::LEVEL_ERROR:
                        std::cerr << Console::Color::RED;
                        std::cerr << "Data\t" << aSize_byte << " bytes";
                        std::cerr << Console::Color::WHITE << std::endl;
                        break;

                    case Level::LEVEL_WARNING:
                        std::cerr << Console::Color::YELLOW;
                        std::cerr << "Data\t" << aSize_byte << " bytes";
                        std::cerr << Console::Color::WHITE << std::endl;
                        break;

                    case Level::LEVEL_INFO:
                    case Level::LEVEL_NOISE:
                        std::cerr << "Data\t" << aSize_byte << "bytes";
                        std::cerr << std::endl;
                        break;

                    default: assert(false);
                    }
                }
            }
        }

        void Log::WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, Level aLevel, unsigned int aFlags)
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
                case Level::LEVEL_ERROR:
                    std::cerr << Console::Color::RED;
                    DisplayInConsole("ERROR", aFile, aFunction, aLine);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case Level::LEVEL_WARNING:
                    std::cerr << Console::Color::YELLOW;
                    DisplayInConsole("WARNING", aFile, aFunction, aLine);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case Level::LEVEL_INFO:
                    DisplayInConsole("INFO", aFile, aFunction, aLine);
                    break;

                case Level::LEVEL_NOISE:
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
                case Level::LEVEL_ERROR:
                    std::cerr << Console::Color::RED;
                    DisplayInConsole(aException);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case Level::LEVEL_WARNING:
                    std::cerr << Console::Color::YELLOW;
                    DisplayInConsole(aException);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case Level::LEVEL_INFO:
                case Level::LEVEL_NOISE:
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
                case Level::LEVEL_ERROR:
                    std::cerr << Console::Color::RED;
                    DisplayInConsole(aMsg);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case Level::LEVEL_WARNING:
                    std::cerr << Console::Color::YELLOW;
                    DisplayInConsole(aMsg);
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case Level::LEVEL_INFO:
                case Level::LEVEL_NOISE:
                    DisplayInConsole(aMsg);
                    break;

                default: assert(false);
                }
            }
        }

        // ===== Configurable ===============================================

        void Log::Config_Set(const Log_Config& aIn)
        {
            Configurable::Config_Set(aIn);

            CloseLogFiles();
            VerifyFolder();
        }

        const char* ConsoleMode_NAMES[] =
        {
            "MODE_DEBUG",
            "MODE_USER",
        };

        Log gLog;

        // Private
        // //////////////////////////////////////////////////////////////////

        void Log::DisplayInConsole(const char* aMsg)
        {
            assert(nullptr != aMsg);

            switch (mConfig.GetConsoleMode())
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

            switch (mConfig.GetConsoleMode())
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
            switch (mConfig.GetConsoleMode())
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
                    lResult = new LogFile(mConfig.GetFolder(), mProcessId, lThreadId);

                    mFiles.insert(FileMap::value_type(lThreadId, lResult));
                }
                catch (Exception eE)
                {
                    // NOT TESTED
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

        void Log::VerifyFolder()
        {
            auto lFolder = mConfig.GetFolder();

            mFileEnabled = 0 < strlen(lFolder);
            if (mFileEnabled)
            {
                struct _stat lStat;

                auto lRet = _stat(lFolder, &lStat);

                mFileEnabled = 0 == lRet;
                if (mFileEnabled)
                {
                    KMS_EXCEPTION_ASSERT(0 != (lStat.st_mode & _S_IFDIR), RESULT_INVALID_CONFIG, "Invalid folder name", lFolder);
                }
            }
        }

    }
}
