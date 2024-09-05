
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/Log.h

#pragma once

namespace KMS
{
    namespace Embedded
    {

        class IFile;

        class LogEntry
        {

        public:

            static const uint8_t LEVEL_ERROR;
            static const uint8_t LEVEL_WARNING;
            static const uint8_t LEVEL_INFO;

            LogEntry();

        // Internal

            uint32_t Get(const char* aMsg, char* aOut, unsigned int aOutSize_byte) const;

            uint16_t GetNumero() const;

            void Set(uint8_t aLevel, uint16_t aNumero, uint32_t aTime, uint32_t aVal0, uint32_t aVal1);

            bool Load(IFile* aFile);
            
            bool Save(IFile* aFile) const;

        private:

            uint32_t mValues[2];
            uint32_t mTime;
            uint16_t mNumero;
            uint8_t  mLevel;
            uint8_t  mReserved0[1];

        };

        class Log
        {

        public:

            Log(LogEntry* aVector, uint16_t aLength, const char** aMessages, uint16_t aQty);

            void Clear();

            void AddEntry(uint8_t aLevel, uint16_t mNumero, uint32_t aTime = 0, uint32_t aVal0 = 0, uint32_t aVal1 = 0);

            uint16_t GetEntryCount() const;

            // aIndex  0  Last entry
            const LogEntry* GetEntry(uint16_t aIndex) const;

            // aIndex  0  Last entry
            uint32_t GetEntry(uint16_t aIndex, char* aOut, unsigned int aOutSize_byte) const;

            void Load(IFile* aFile);

            bool Save(IFile* aFile);

        private:

            uint16_t mCount;
            uint16_t mLast;

            const char** mMessages;
            uint16_t     mMessageQty;

            LogEntry* mVector;
            uint16_t  mVectorLength;

        };

    }
}
