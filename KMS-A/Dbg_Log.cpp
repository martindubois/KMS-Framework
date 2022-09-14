
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log.cpp

// TEST COVERAGE 2022-09-06 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/Environment.h>
#include <KMS/OS.h>
#include <KMS/Proc/Process.h>

#include <KMS/Dbg/Log.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static KMS::Dbg::LogFile::Level ToLevel(const char* aIn);

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Log::Log() : mCounter(0), mEntryLevel(LogFile::Level::LEVEL_NOISE), mProcessId(OS::GetProcessId())
        {
            ResetConsoleLevel();
            ResetFileLevel   ();
            ResetFolder      ();
        }

        Log::~Log() { CloseLogFiles(); }

        bool Log::IsFileEnabled() const { return mEnabled; }

        void Log::ResetConsoleLevel() { SetConsoleLevel(LogFile::Level::LEVEL_WARNING); }
        void Log::ResetFileLevel   () { SetFileLevel   (LogFile::Level::LEVEL_INFO   ); }

        void Log::ResetFolder()
        {
            File::Folder lHome(File::Folder::Id::HOME);

            SetFolder(File::Folder(lHome, "KMS-Framework"));
        }

        void Log::SetConsoleLevel(LogFile::Level aL) { mConsoleLevel = aL; }
        void Log::SetFileLevel   (LogFile::Level aL) { mFileLevel    = aL; }

        void Log::SetFolder(const File::Folder& aF)
        {
            mFolder = aF;

            mEnabled = mFolder.DoesExist();

            CloseLogFiles();
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

        // ===== Cfg::Configurables =========================================

        void Log::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "===== KMS::Dbg::Log =====\n"
                "ConsoleLevel\n"
                "    Set the console level to the default value\n"
                "    Default: WARNING\n"
                "ConsoleLevel = NONE|ERROR|WARNING|INFO|NOISE\n"
                "    Set the level\n"
                "FileLevel\n"
                "    Set the file level to the default value\n"
                "    Default: INFO\n"
                "FileLevel = NONE|ERROR|WARNING|INFO|NOISE\n"
                "    Set the level\n"
                "Folder\n"
                "    Set the folde to the default value\n"
                "    Default: ~/KMS-Framework\n"
                "Folder = {Value}\n"
                "    Set the folder\n");

            Cfg::Configurable::DisplayHelp(aOut);
        }

        bool Log::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("ConsoleLevel") { ResetConsoleLevel(); return true; }
                CFG_IF("FileLevel"   ) { ResetFileLevel   (); return true; }
                CFG_IF("Folder"      ) { ResetFolder      (); return true; }
            }
            else
            {
                char lE[PATH_LENGTH];

                CFG_CONVERT("ConsoleLevel", SetConsoleLevel, ToLevel);
                CFG_CONVERT("FileLevel"   , SetFileLevel   , ToLevel);

                CFG_EXPAND("Folder", SetFolder);
            }

            return Cfg::Configurable::SetAttribute(aA, aV);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Log::CloseLogFiles()
        {
            for (const FileMap::value_type& lVT : mFiles)
            {
                delete lVT.second;
            }

            mFiles.clear();
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

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

KMS::Dbg::LogFile::Level ToLevel(const char* aIn)
{
    if (0 == _stricmp("NONE"   , aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_NONE   ; }
    if (0 == _stricmp("ERROR"  , aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_ERROR  ; }
    if (0 == _stricmp("WARNING", aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_WARNING; }
    if (0 == _stricmp("INFO"   , aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_INFO   ; }
    if (0 == _stricmp("NOISE"  , aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_NOISE  ; }

    KMS_EXCEPTION_WITH_INFO(CONFIG_VALUE, "Invalid level name", aIn);
}
