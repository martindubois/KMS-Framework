
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Embedded_Log.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/IFile.h>

#include <KMS/Embedded/Log.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define LEVEL_INVALID  (0xff)
#define NUMERO_INVALID (0xffff)
#define TIME_INVALID   (0)

static const char* LEVEL_NAMES[] = { "ERROR", "WARNING", "INFO" };

namespace KMS
{
    namespace Embedded
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const uint8_t LogEntry::LEVEL_ERROR   = 0;
        const uint8_t LogEntry::LEVEL_WARNING = 1;
        const uint8_t LogEntry::LEVEL_INFO    = 2;

        LogEntry::LogEntry() : mTime(TIME_INVALID), mNumero(NUMERO_INVALID), mLevel(LEVEL_INVALID)
        {}

        Log::Log(LogEntry* aVector, uint16_t aLength, const char** aMessages, uint16_t aQty)
            : mCount(0)
            , mLast(0)
            , mMessages(aMessages)
            , mMessageQty(aQty)
            , mVector(aVector)
            , mVectorLength(aLength)
        {}

        void Log::Clear() { mCount = 0; mLast  = 0; }

        void Log::AddEntry(uint8_t aLevel, uint16_t aNumero, uint32_t aTime, uint32_t aVal0, uint32_t aVal1)
        {
            if (0 == mCount)
            {
                mCount++;
                mLast = 0;
            }
            else
            {
                mLast = (mLast + 1) % mVectorLength;

                if (mVectorLength > mCount)
                {
                    mCount++;
                }
            }

            auto lEntry = mVector + mLast;

            lEntry->Set(aLevel, aNumero, aTime, aVal0, aVal1);
        }

        uint16_t Log::GetEntryCount() const { return mCount; }

        const LogEntry* Log::GetEntry(uint16_t aIndex) const
        {
            LogEntry* lResult = nullptr;

            if (mCount > aIndex)
            {
                if (mLast >= aIndex)
                {
                    lResult = mVector + mLast - aIndex;
                }
                else
                {
                    lResult = mVector + mVectorLength + mLast - aIndex;
                }
            }

            return lResult;
        }

        uint32_t Log::GetEntry(uint16_t aIndex, char* aOut, unsigned int aOutSize_byte) const
        {
            uint32_t lResult = TIME_INVALID;

            aOut[0] = '\0';

            auto lEntry = GetEntry(aIndex);
            if (nullptr != lEntry)
            {
                auto lNumero = lEntry->GetNumero();

                lResult = lEntry->Get((mMessageQty > lNumero) ? mMessages[lNumero] : "", aOut, aOutSize_byte);
            }

            return lResult;
        }

        void Log::Load(IFile* aFile)
        {
            for (uint16_t i = 0; i < mVectorLength; i++)
            {
                if (!mVector[i].Load(aFile))
                {
                    break;
                }

                mCount++;
                mLast++;
            }
        }

        bool Log::Save(IFile* aFile)
        {
            bool lResult = true;

            auto lIndex = mCount;

            while ((0 < lIndex) && lResult)
            {
                lIndex--;

                auto lEntry = GetEntry(lIndex);

                lResult = lEntry->Save(aFile);
            }

            return lResult;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        uint32_t LogEntry::Get(const char* aMsg, char* aOut, unsigned int aOutSize_byte) const
        {
            if ((LEVEL_INVALID == mLevel) || (NUMERO_INVALID == mNumero))
            {
                return TIME_INVALID;
            }

            auto lRet = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s %u ", LEVEL_NAMES[mLevel], mNumero);

            sprintf_s(aOut + lRet SizeInfo(aOutSize_byte - lRet), aMsg, mValues[0], mValues[1]);

            return mTime;
        }

        uint16_t LogEntry::GetNumero() const { return mNumero; }

        void LogEntry::Set(uint8_t aLevel, uint16_t aNumero, uint32_t aTime, uint32_t aVal0, uint32_t aVal1)
        {
            mLevel     = aLevel;
            mNumero    = aNumero;
            mTime      = aTime;
            mValues[0] = aVal0;
            mValues[1] = aVal1;
        }

        bool LogEntry::Load(IFile* aFile)
        {
            auto lRet_byte = aFile->Read(this, sizeof(LogEntry));

            return sizeof(LogEntry) == lRet_byte;
        }

        bool LogEntry::Save(IFile* aFile) const
        {
            auto lRet_byte = aFile->Write(this, sizeof(LogEntry));

            return sizeof(LogEntry) == lRet_byte;
        }

    }
}

