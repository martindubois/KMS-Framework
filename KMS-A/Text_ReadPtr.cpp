
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_ReadPtr.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Text/ReadPtr.h>

KMS_RESULT_STATIC(RESULT_TOO_SHORT);

namespace KMS
{
    namespace Text
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        ReadPtr::ReadPtr(const char* aBase, unsigned int aSize_byte) : mBase(aBase), mIndex_byte(0), mSize_byte(aSize_byte)
        {
            assert(nullptr != aBase);
        }

        ReadPtr::operator const char* () const
        {
            assert(nullptr != mBase);

            return mBase + mIndex_byte;
        }

        void ReadPtr::operator += (unsigned int aA)
        {
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mSize_byte >= mIndex_byte + aA, RESULT_TOO_SHORT, "Incomplete text", aA);

            mIndex_byte += aA;
        }

        unsigned int ReadPtr::GetIndex() const { return mIndex_byte; }

        bool ReadPtr::IsAtEnd() const { return mSize_byte <= mIndex_byte; }

        unsigned int ReadPtr::ExtractUntil(char aC, char* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);

            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            unsigned int lResult_byte = 0;

            while (mSize_byte > mIndex_byte)
            {
                KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aC);

                if (aC == mBase[mIndex_byte])
                {
                    break;
                }

                aOut[lResult_byte] = mBase[mIndex_byte]; lResult_byte++; mIndex_byte++;
            }

            aOut[lResult_byte] = '\0';

            return lResult_byte;
        }

        unsigned int ReadPtr::ExtractUntil(const char* aStr, char* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aStr);
            assert(nullptr != aOut);

            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            unsigned int lResult_byte = 0;

            while (mSize_byte > mIndex_byte)
            {
                KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aStr);

                if (NULL != strchr(aStr, mBase[mIndex_byte]))
                {
                    break;
                }

                aOut[lResult_byte] = mBase[mIndex_byte]; lResult_byte++; mIndex_byte++;
            }

            aOut[lResult_byte] = '\0';

            return lResult_byte;
        }

        unsigned int ReadPtr::ExtractWhile(const char* aStr, char* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aStr);
            assert(nullptr != aOut);

            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            unsigned int lResult_byte = 0;

            while (mSize_byte > mIndex_byte)
            {
                KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aStr);

                if (NULL == strchr(aStr, mBase[mIndex_byte]))
                {
                    break;
                }

                aOut[lResult_byte] = mBase[mIndex_byte]; lResult_byte++; mIndex_byte++;
            }

            aOut[lResult_byte] = '\0';

            return lResult_byte;
        }

        void ReadPtr::Skip(const char* aStr)
        {
            assert(nullptr != aStr);

            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            while (mSize_byte > mIndex_byte)
            {
                if (NULL == strchr(aStr, mBase[mIndex_byte]))
                {
                    break;
                }

                mIndex_byte++;
            }
        }

        void ReadPtr::SkipBlank() { Skip(" \n\r\t"); }

        void ReadPtr::Test(char aC)
        {
            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mSize_byte > mIndex_byte, RESULT_TOO_SHORT, "Incomplete text", aC);

            KMS_EXCEPTION_ASSERT(aC == mBase[mIndex_byte], RESULT_INVALID_FORMAT, "Invalid text format", aC);

            mIndex_byte++;
        }

        void ReadPtr::Test(const char* aStr)
        {
            assert(nullptr != aStr);

            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mSize_byte > mIndex_byte, RESULT_TOO_SHORT, "Incomplete text", aStr);

            if (NULL == strchr(aStr, mBase[mIndex_byte]))
            {
                KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid text format", aStr);
            }

            mIndex_byte++;
        }

    }
}
