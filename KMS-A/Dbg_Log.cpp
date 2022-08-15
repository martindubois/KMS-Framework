
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log.cpp

// TODO Add a Level attribute with a default value of LEVEL_INFO. Only log
//      information of this LEVEL or lower.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Environment.h>
#include <KMS/OS.h>
#include <KMS/Proc/Process.h>
#include <KMS/Thread/Thread.h>

#include <KMS/Dbg/Log.h>

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Log::Log() : mCounter(0), mProcessId(OS::GetProcessId())
        {
            ResetFolder();
        }

        Log::~Log()
        {
            for (const FileMap::value_type& lVT : mFiles)
            {
                delete lVT.second;
            }
        }

        bool Log::IsEnabled() const { return mEnabled; }

        void Log::ResetFolder()
        {
            File::Folder lHome(File::Folder::Id::HOME);

            mFolder = File::Folder(lHome, "KMS-Framework");

            mEnabled = mFolder.DoesExist();
        }

        void Log::SetFolder(const File::Folder& aF) { mFolder = aF; }

        void Log::WriteEntry(const char* aFile, const char* aFunction, unsigned int aLine, Dbg::LogFile::Level aLevel)
        {
            if (mEnabled)
            {
                LogFile* lLF = FindLogFile();
                assert(NULL != lLF);

                mCounter++;

                lLF->WriteEntry(mCounter, aFile, aFunction, aLine, aLevel);
            }
        }

        void Log::WriteException(const Exception& aException)
        {
            if (mEnabled)
            {
                LogFile* lLF = FindLogFile();
                assert(NULL != lLF);

                lLF->WriteException(aException);
            }
        }

        void Log::WriteMessage(const char* aMsg)
        {
            if (mEnabled)
            {
                LogFile* lLF = FindLogFile();
                assert(NULL != lLF);

                lLF->WriteMessage(aMsg);
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
                "    Set the folder\n");

            Cfg::Configurable::DisplayHelp(aOut);
        }

        bool Log::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Folder") { ResetFolder(); return true; }
            }
            else
            {
                char lE[PATH_LENGTH];

                CFG_EXPAND("Folder", SetFolder);
            }

            return Cfg::Configurable::SetAttribute(aA, aV);
        }

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
