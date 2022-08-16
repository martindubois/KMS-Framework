
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/Environment.h>
#include <KMS/OS.h>
#include <KMS/Proc/Process.h>
#include <KMS/Thread/Thread.h>

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
            ResetFolder();
            ResetLevel();
        }

        Log::~Log() { CloseLogFiles(); }

        bool Log::IsEnabled() const { return mEnabled; }

        void Log::ResetFolder()
        {
            File::Folder lHome(File::Folder::Id::HOME);

            SetFolder(File::Folder(lHome, "KMS-Framework"));
        }

        void Log::ResetLevel() { SetLevel(LogFile::Level::LEVEL_INFO); }

        void Log::SetFolder(const File::Folder& aF)
        {
            mFolder = aF;

            mEnabled = mFolder.DoesExist();

            CloseLogFiles();
        }

        void Log::SetLevel(LogFile::Level aL) { mLevel = aL; }

        void Log::WriteData(const void* aData, unsigned int aSize_byte)
        {
            if (mEnabled)
            {
                if (mLevel >= mEntryLevel)
                {
                    LogFile* lLF = FindLogFile();
                    assert(NULL != lLF);

                    lLF->WriteData(aData, aSize_byte);
                }
            }
            else
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << KMS::Console::Color::RED;
                    std::cerr << "D\t" << aSize_byte << "\t";
                    std::cerr << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;
                }
            }
        }

        void Log::WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, Dbg::LogFile::Level aLevel)
        {
            mEntryLevel = aLevel;

            mCounter++;

            if (mEnabled)
            {
                if (mLevel >= mEntryLevel)
                {
                    LogFile* lLF = FindLogFile();
                    assert(NULL != lLF);

                    lLF->WriteEntry(mCounter, aFile, aFunction, aLine, aLevel);
                }
            }
            else
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << KMS::Console::Color::RED;
                    std::cerr << "E\t" << mCounter << "\t" << aLine << "\t" << aFunction << "\t" << aFile << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;
                }
            }
        }

        void Log::WriteException(const Exception& aException)
        {
            if (mEnabled)
            {
                if (mLevel >= mEntryLevel)
                {
                    LogFile* lLF = FindLogFile();
                    assert(NULL != lLF);

                    lLF->WriteException(aException);
                }
            }
            else
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << KMS::Console::Color::RED;
                    std::cerr << "X" << std::endl;
                    std::cerr << aException << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;
                }
            }
        }

        void Log::WriteMessage(const char* aMsg)
        {
            if (mEnabled)
            {
                if (mLevel >= mEntryLevel)
                {
                    LogFile* lLF = FindLogFile();
                    assert(NULL != lLF);

                    lLF->WriteMessage(aMsg);
                }
            }
            else
            {
                switch (mEntryLevel)
                {
                case LogFile::Level::LEVEL_ERROR:
                    std::cerr << KMS::Console::Color::RED;
                    std::cerr << "M\t\"" << aMsg << "\"" << std::endl;
                    std::cerr << KMS::Console::Color::WHITE;
                    break;
                }
            }
        }

        Log gLog;

        // ===== Cfg::Configurables =========================================

        void Log::DisplayHelp(FILE* aOut)
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "===== KMS::Dbg::Log =====\n"
                "Folder\n"
                "    Set the folde to the default value\n"
                "    Default: ~/KMS-Framework\n"
                "Folder = {Value}\n"
                "    Set the folder\n"
                "Level\n"
                "    Set the level to the default value\n"
                "    Default: INFO\n"
                "Level = ERROR|WARNING|INFO|NOISE\n"
                "    Set the level\n");

            Cfg::Configurable::DisplayHelp(aOut);
        }

        bool Log::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Folder") { ResetFolder(); return true; }
                CFG_IF("Level" ) { ResetLevel (); return true; }
            }
            else
            {
                char lE[PATH_LENGTH];

                CFG_EXPAND("Folder", SetFolder);

                CFG_CONVERT("Level", SetLevel, ToLevel);
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
    if (0 == _stricmp("ERROR"  , aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_ERROR  ; }
    if (0 == _stricmp("WARNING", aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_WARNING; }
    if (0 == _stricmp("INFO"   , aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_INFO   ; }
    if (0 == _stricmp("NOISE"  , aIn)) { return KMS::Dbg::LogFile::Level::LEVEL_NOISE  ; }

    KMS_EXCEPTION_WITH_INFO(CONFIG_VALUE, "Invalid level name", aIn);
}
