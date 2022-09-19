
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_ReadPtr.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Text/ReadPtr.h>

namespace KMS
{
    namespace Text
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        ReadPtr::ReadPtr(const char* aBase, unsigned int aSize_byte) : mBase(aBase), mIndex_byte(0), mSize_byte(aSize_byte)
        {
            assert(NULL != aBase);
        }

        ReadPtr::operator const char* () const
        {
            assert(NULL != mBase);

            return mBase + mIndex_byte;
        }

        void ReadPtr::operator += (unsigned int aA)
        {
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mSize_byte >= mIndex_byte + aA, TEXT_TOO_SHORT, "Incomplete text");

            mIndex_byte += aA;
        }

        unsigned int ReadPtr::GetIndex() const { return mIndex_byte; }

        bool ReadPtr::IsAtEnd() const { return mSize_byte <= mIndex_byte; }

        unsigned int ReadPtr::ExtractUntil(char aC, char* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aOut);

            assert(NULL != mBase);
            assert(mIndex_byte <= mSize_byte);

            unsigned int lResult_byte = 0;

            while (mSize_byte > mIndex_byte)
            {
                KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, OUTPUT_TOO_SHORT, "The output buffer is too short");

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
            assert(NULL != aStr);
            assert(NULL != aOut);

            assert(NULL != mBase);
            assert(mIndex_byte <= mSize_byte);

            unsigned int lResult_byte = 0;

            while (mSize_byte > mIndex_byte)
            {
                KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, OUTPUT_TOO_SHORT, "The output buffer is too short");

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
            assert(NULL != aStr);
            assert(NULL != aOut);

            assert(NULL != mBase);
            assert(mIndex_byte <= mSize_byte);

            unsigned int lResult_byte = 0;

            while (mSize_byte > mIndex_byte)
            {
                if (aOutSize_byte < lResult_byte + 2)
                {
                    KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output buffer is too short");
                }

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
            assert(NULL != aStr);

            assert(NULL != mBase);
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
            assert(NULL != mBase);
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mSize_byte > mIndex_byte, TEXT_TOO_SHORT, "Incomplete text");

            KMS_EXCEPTION_ASSERT(aC == mBase[mIndex_byte], TEXT_FORMAT, "Invalid text format");

            mIndex_byte++;
        }

        void ReadPtr::Test(const char* aStr)
        {
            assert(NULL != aStr);

            assert(NULL != mBase);
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mSize_byte > mIndex_byte, TEXT_TOO_SHORT, "Incomplete text");

            if (NULL == strchr(aStr, mBase[mIndex_byte]))
            {
                KMS_EXCEPTION(TEXT_FORMAT, "Invalid text format");
            }

            mIndex_byte++;
        }

    }
}
