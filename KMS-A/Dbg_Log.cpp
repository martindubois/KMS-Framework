
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log.cpp

// TEST COVERAGE 2022-09-06 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/OS.h>
#include <KMS/Proc/Process.h>

#include <KMS/Dbg/Log.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_CONSOLE_LEVEL (LogFile::Level::LEVEL_WARNING)
#define DEFAULT_CONSOLE_MODE  (Log::ConsoleMode::USER)
#define DEFAULT_FILE_LEVEL    (LogFile::Level::LEVEL_INFO)
#define DEFAULT_FOLDER_NAME   ("KMS-Framework")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_CONSOLE_LEVEL("ConsoleLevel = NOISE | INFO | WARNING | ERROR | NONE");
static const KMS::Cfg::MetaData MD_CONSOLE_MODE ("ConsoleMode = DEBUG | USER");
static const KMS::Cfg::MetaData MD_FILE_LEVEL   ("FileLevel = NOISE | INFO | WARNING | ERROR | NONE");
static const KMS::Cfg::MetaData MD_FOLDER       ("Folder = {Path}");

#define ON_FOLDER_CHANGED (1)

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Log::CONSOLE_MODE_NAMES[] = { "DEBUG", "USER" };

        const unsigned int Log::FLAG_USER_REDUNDANT = 0x00000001;

        Log::Log()
            : mConsoleLevel(DEFAULT_CONSOLE_LEVEL)
            , mConsoleMode (DEFAULT_CONSOLE_MODE)
            , mFileLevel   (DEFAULT_FILE_LEVEL)
            , mFolder      (File::Folder(File::Folder::Id::HOME, DEFAULT_FOLDER_NAME))
            , mCounter(0)
            , mEntryLevel(LogFile::Level::LEVEL_NOISE)
            , mProcessId(OS::GetProcessId())
        {
            memset(&mHideCounts, 0, sizeof(mHideCounts));

            mFolder.mOnChanged.Set(this, ON_FOLDER_CHANGED);

            AddEntry("ConsoleLevel", &mConsoleLevel, false, &MD_CONSOLE_LEVEL);
            AddEntry("ConsoleMode" , &mConsoleMode , false, &MD_CONSOLE_MODE);
            AddEntry("FileLevel"   , &mFileLevel   , false, &MD_FILE_LEVEL);
            AddEntry("Folder"      , &mFolder      , false, &MD_FOLDER);

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
            for (const FileMap::value_type& lVT : mFiles)
            {
                delete lVT.second;
            }

            mFiles.clear();

            mEnabled = mFolder.Get().DoesExist();
        }

        #define IF_FILE       if (mEnabled && (mFileLevel >= mEntryLevel))
        #define IF_CONSOLE(F) if ((0 == ((F) & FLAG_USER_REDUNDANT)) && (((!mEnabled) && (LogFile::Level::LEVEL_WARNING >= mEntryLevel)) || (mConsoleLevel >= mEntryLevel)))

        void Log::WriteData(const void* aData, unsigned int aSize_byte, unsigned int aFlags)
        {
            IF_FILE
            {
                LogFile* lLF = FindLogFile();
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
                LogFile* lLF = FindLogFile();
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
                LogFile* lLF = FindLogFile();
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
                LogFile* lLF = FindLogFile();
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
                    std::cerr << "Message\t\"" << aMsg << "\"";
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case LogFile::Level::LEVEL_WARNING:
                    std::cerr << Console::Color::YELLOW;
                    std::cerr << "Message\t\"" << aMsg << "\"";
                    std::cerr << Console::Color::WHITE << std::endl;
                    break;

                case LogFile::Level::LEVEL_INFO:
                case LogFile::Level::LEVEL_NOISE:
                    // NOT TESTED
                    std::cerr << "Message\t\"" << aMsg << "\"" << std::endl;
                    break;

                default: assert(false);
                }
            }
        }

        // ===== Msg::IReceiver =============================================

        unsigned int Log::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

            switch (aCode)
            {
            case ON_FOLDER_CHANGED: CloseLogFiles(); lResult = 0; break;

            default: assert(false);
            }

            return lResult;
        }

        Log gLog;

        // Private
        // //////////////////////////////////////////////////////////////////

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

            unsigned int lThreadId = OS::GetThreadId();

            FileMap::iterator lIt = mFiles.find(lThreadId);
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
