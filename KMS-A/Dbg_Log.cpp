
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_CONSOLE_LEVEL("ConsoleLevel = NOISE | INFO | WARNING | ERROR | NONE");
static const KMS::Cfg::MetaData MD_FILE_LEVEL   ("FileLevel = NOISE | INFO | WARNING | ERROR | NONE");
static const KMS::Cfg::MetaData MD_FOLDER       ("Folder = {Path}");

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Log::Log()
            : mConsoleLevel(LogFile::Level::LEVEL_WARNING)
            , mFileLevel   (LogFile::Level::LEVEL_INFO)
            , mFolder      (File::Folder(File::Folder::Id::HOME, "KMS-Framework"))
            , mCounter(0)
            , mEntryLevel(LogFile::Level::LEVEL_NOISE)
            , mProcessId(OS::GetProcessId())
        {
            AddEntry("ConsoleLevel", &mConsoleLevel, false, &MD_CONSOLE_LEVEL);
            AddEntry("FileLevel"   , &mFileLevel   , false, &MD_FILE_LEVEL);
            AddEntry("Folder"      , &mFolder      , false, &MD_FOLDER);

            CloseLogFiles();
        }

        Log::~Log() { CloseLogFiles(); }

        bool Log::IsFileEnabled() const { return mEnabled; }

        void Log::CloseLogFiles()
        {
            for (const FileMap::value_type& lVT : mFiles)
            {
                delete lVT.second;
            }

            mFiles.clear();

            mEnabled = mFolder.Get().DoesExist();
        }

        #define IF_FILE    if (mEnabled && (mFileLevel >= mEntryLevel))
        #define IF_CONSOLE if (((!mEnabled) && LogFile::Level::LEVEL_WARNING >= mEntryLevel) || (mConsoleLevel >= mEntryLevel))

        void Log::WriteData(const void* aData, unsigned int aSize_byte)
        {
            IF_FILE
            {
                LogFile* lLF = FindLogFile();
                assert(NULL != lLF);

                lLF->WriteData(aData, aSize_byte);
            }

            IF_CONSOLE
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << KMS::Console::Color::RED;
                    std::cerr << "D\t" << aSize_byte << "\t";
                    std::cerr << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;

                case LogFile::Level::LEVEL_WARNING:
                    std::cerr << KMS::Console::Color::YELLOW;
                    std::cerr << "D\t" << aSize_byte << "\t";
                    std::cerr << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;

                case LogFile::Level::LEVEL_INFO:
                case LogFile::Level::LEVEL_NOISE:
                    // NOT TESTED
                    std::cerr << "D\t" << aSize_byte << "\t";
                    std::cerr << std::endl;
                    break;

                default: assert(false);
                }
            }
        }

        void Log::WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, Dbg::LogFile::Level aLevel)
        {
            mEntryLevel = aLevel;

            mCounter++;

            IF_FILE
            {
                LogFile* lLF = FindLogFile();
                assert(NULL != lLF);

                lLF->WriteEntry(mCounter, aFile, aFunction, aLine, aLevel);
            }

            IF_CONSOLE
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << KMS::Console::Color::RED;
                    std::cerr << "E\t" << mCounter << "\t" << aLine << "\t" << aFunction << "\t" << aFile << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;

                case LogFile::Level::LEVEL_WARNING:
                    std::cerr << KMS::Console::Color::YELLOW;
                    std::cerr << "W\t" << mCounter << "\t" << aLine << "\t" << aFunction << "\t" << aFile << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;

                case LogFile::Level::LEVEL_INFO:
                    // NOT TESTED
                    std::cerr << "I\t" << mCounter << "\t" << aLine << "\t" << aFunction << "\t" << aFile << std::endl;
                    break;

                case LogFile::Level::LEVEL_NOISE:
                    // NOT TESTED
                    std::cerr << "N\t" << mCounter << "\t" << aLine << "\t" << aFunction << "\t" << aFile << std::endl;
                    break;

                default: assert(false);
                }
            }
        }

        void Log::WriteException(const Exception& aException)
        {
            IF_FILE
            {
                LogFile* lLF = FindLogFile();
                assert(NULL != lLF);

                lLF->WriteException(aException);
            }

            IF_CONSOLE
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << KMS::Console::Color::RED;
                    std::cerr << "X" << std::endl;
                    std::cerr << aException << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;

                case LogFile::Level::LEVEL_WARNING:
                    std::cerr << KMS::Console::Color::YELLOW;
                    std::cerr << "X" << std::endl;
                    std::cerr << aException << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;

                case LogFile::Level::LEVEL_INFO:
                case LogFile::Level::LEVEL_NOISE:
                    // NOT TESTED
                    std::cerr << "X" << std::endl;
                    std::cerr << aException << std::endl;
                    break;

                default: assert(false);
                }
            }
        }

        void Log::WriteMessage(const char* aMsg)
        {
            IF_FILE
            {
                LogFile* lLF = FindLogFile();
                assert(NULL != lLF);

                lLF->WriteMessage(aMsg);
            }

            IF_CONSOLE
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << KMS::Console::Color::RED;
                    std::cerr << "M\t\"" << aMsg << "\"" << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;

                case LogFile::Level::LEVEL_WARNING:
                    std::cerr << KMS::Console::Color::YELLOW;
                    std::cerr << "M\t\"" << aMsg << "\"" << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;

                case LogFile::Level::LEVEL_INFO:
                case LogFile::Level::LEVEL_NOISE:
                    // NOT TESTED
                    std::cerr << "M\t\"" << aMsg << "\"" << std::endl;
                    break;

                default: assert(false);
                }
            }
        }

        Log gLog;

        // Private
        // //////////////////////////////////////////////////////////////////

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
