
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_LogFile.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/OS.h>

#include <KMS/Dbg/LogFile.h>

// ===== KMS-A ==============================================================
#include "../Common/Version.h"

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        LogFile::LogFile(const File::Folder& aFolder, unsigned int aProcessId, unsigned int aThreadId)
        {
            char lFileName[PATH_LENGTH];

            sprintf_s(lFileName, "%08x-%08x.txt", aProcessId, aThreadId);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(lFileName, lPath, sizeof(lPath));

            int lRet = fopen_s(&mFile, lPath, "wb");
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(FILE_OPEN, "Cannot open log file", lPath);
            }

            WriteEntry(0, __FILE__, __FUNCTION__, __LINE__, Level::LEVEL_INFO);
            WriteVersion(VERSION);
        }

        LogFile::~LogFile()
        {
            assert(NULL != mFile);

            uint64_t lTimeStamp = OS::GetSystemTime();

            fprintf(mFile, "Z\t%016llx\n", lTimeStamp);

            fclose(mFile);
        }

        void LogFile::WriteData(const void* aData, unsigned int aSize_byte)
        {
            assert(NULL != aData);

            const uint8_t* lData = reinterpret_cast<const uint8_t*>(aData);

            fprintf(mFile, "D\t%u\t", aSize_byte);

            for (unsigned int i = 0; i < aSize_byte; i++)
            {
                fprintf(mFile, " %02x", lData[i]);
            }

            fprintf(mFile, "\n");
        }

        void LogFile::WriteEntry(unsigned int aCounter, const char* aFile, const char* aFunction, unsigned int aLine, Level aLevel)
        {
            assert(NULL != aFile);
            assert(NULL != aFunction);
            assert(0 < aLine);

            assert(NULL != mFile);

            char lET;

            switch (aLevel)
            {
            case Level::LEVEL_ERROR  : lET = static_cast<char>(EntryType::TYPE_ERROR  ); break;
            case Level::LEVEL_WARNING: lET = static_cast<char>(EntryType::TYPE_WARNING); break;
            case Level::LEVEL_INFO   : lET = static_cast<char>(EntryType::TYPE_INFO   ); break;
            case Level::LEVEL_NOISE  : lET = static_cast<char>(EntryType::TYPE_NOISE  ); break;

            default: assert(false);
            }

            uint64_t lTimeStamp = OS::GetSystemTime();

            fprintf(mFile, "%c\t%016llx\t%u\t%u\t%s\t%s\n",
                lET,
                lTimeStamp,
                aCounter,
                aLine,
                aFunction,
                aFile);
            fflush(mFile);
        }

        void LogFile::WriteException(const Exception& aException)
        {
            assert(NULL != mFile);

            fprintf(mFile, "X\t%s\t\"%s\"\t%u\t%s\t%s\t\"%s\"\n",
                aException.GetCodeName(),
                aException.what(),
                aException.GetLine(),
                aException.GetFunction(),
                aException.GetFile(),
                aException.GetInfo());
            fflush(mFile);
        }

        void LogFile::WriteMessage(const char* aMsg)
        {
            assert(NULL != aMsg);

            assert(NULL != mFile);

            fprintf(mFile, "M\t\"%s\"\n", aMsg);
            fflush(mFile);
        }

        void LogFile::WriteVersion(const Version& aV)
        {
            assert(NULL != mFile);

            fprintf(mFile, "V\t%u.%u.%u.%u\t\"%s\"\t%s\t%s\n",
                aV.GetMajor(), aV.GetMinor(), aV.GetBuild(), aV.GetCompat(),
                aV.GetDate(),
                aV.GetDebug() ? "Debug" : "Release",
                aV.GetType());
            fflush(mFile);
        }

    }
}
