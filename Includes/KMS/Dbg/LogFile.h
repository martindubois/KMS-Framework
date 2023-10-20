
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/LogFile.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C ==================================================================
#include <stdio.h>

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

namespace KMS
{

    class Exception;
    class Version;

    namespace Dbg
    {

        class LogFile final
        {

        public:

            enum class Level
            {
                LEVEL_NONE   ,
                LEVEL_ERROR  ,
                LEVEL_WARNING,
                LEVEL_INFO   ,
                LEVEL_NOISE  ,

                QTY
            };

            enum class EntryType
            {
                TYPE_DATA      = 'D',
                TYPE_ERROR     = 'E',
                TYPE_INFO      = 'I',
                TYPE_MESSAGE   = 'M',
                TYPE_NOISE     = 'N',
                TYPE_EXCEPTION = 'X',
                TYPE_WARNING   = 'W',

                TYPE_END_OF_FILE = 'Z'
            };

            static const char* LEVEL_NAMES[];

            LogFile(const File::Folder& aFolder, unsigned int aProcessId, unsigned int aThreadId);

            LogFile(const File::Folder& aFolder, const char* aFile);

            ~LogFile();

            unsigned int GetCounter() const;

            const void * GetData() const;

            unsigned int GetDataSize() const;

            EntryType GetEntryType() const;

            const char* GetFunction() const;

            Level GetLevel() const;

            unsigned int GetLine() const;

            const char* GetMessage() const;

            const char* GetSourceFile() const;

            uint64_t GetTimestamp() const;

            bool ReadEntry(Level aLevel);

            void WriteData(const void* aData, unsigned int aSize_byte);

            void WriteEntry(unsigned int aCounter, const char* aFile, const char* aFunction, unsigned int aLine, Level aLevel);

            void WriteException(const Exception& aException);

            void WriteMessage(const char* aMsg);

            void WriteVersion(const Version& aV);

        private:

            // D {Size_byte} {Data}
            // E {Timestamp} {Counter} {Line} {Function} {SourceFile}
            // I {Timestamp} {Counter} {Line} {Function} {SourceFile}
            // M {Message}
            // N {Timestamp} {Counter} {Line} {Function} {SourceFile}
            // W {Timestamp} {Counter} {Line} {Function} {SourceFile}
            // X {CodeName} {Message} {Line} {Function} {File} {Info}
            // Z {Timestamp}

            NO_COPY(LogFile);

            uint8_t      mBuffer[256];
            unsigned int mCounter;
            unsigned int mDataSize_byte;
            EntryType    mEntryType;
            FILE       * mFile;
            std::string  mFunction;
            Level        mLevel;
            unsigned int mLine;
            std::string  mMessage;
            std::string  mSourceFile;
            uint64_t     mTimestamp;

        };

    }
}

// Macros
// //////////////////////////////////////////////////////////////////////////

#define KMS_DBG_LOG_ENTRY() KMS::Dbg::gLog.AddEntry(__FILE__, __FUNCTION__, __LINE__)
