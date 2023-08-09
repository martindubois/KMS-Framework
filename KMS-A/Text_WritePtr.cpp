
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_WritePtr.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Text/WritePtr.h>

namespace KMS
{
    namespace Text
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        WritePtr::WritePtr(char* aBase, unsigned int aSize_byte) : mBase(aBase), mIndex_byte(0), mSize_byte(aSize_byte)
        {
            assert(nullptr != aBase);
        }

        WritePtr::~WritePtr()
        {
            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            if (mIndex_byte < mSize_byte)
            {
                mBase[mIndex_byte] = '\0';
            }
        }

        WritePtr::operator char* ()
        {
            assert(nullptr != mBase);

            return mBase + mIndex_byte;
        }

        void WritePtr::operator += (unsigned int aInc)
        {
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mIndex_byte + aInc + 2 <= mSize_byte, TEXT_OUTPUT_TOO_SHORT, "The output buffer is too short", aInc);

            mIndex_byte += aInc;
        }

        unsigned int WritePtr::GetIndex() const { return mIndex_byte; }

        unsigned int WritePtr::GetRemainingSize() const
        {
            assert(mIndex_byte <= mSize_byte);

            return mSize_byte - mIndex_byte;
        }

        void WritePtr::Write(char aIn)
        {
            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mIndex_byte + 1 + 2 <= mSize_byte, TEXT_OUTPUT_TOO_SHORT, "The output buffer is too short", aIn);

            mBase[mIndex_byte] = aIn;

            mIndex_byte++;
        }

        void WritePtr::Write(const char* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != aIn);

            assert(nullptr != mBase);
            assert(mIndex_byte <= mSize_byte);

            KMS_EXCEPTION_ASSERT(mIndex_byte + aInSize_byte + 2 <= mSize_byte, TEXT_OUTPUT_TOO_SHORT, "The output buffer is too short", aIn);

            memcpy(mBase + mIndex_byte, aIn, aInSize_byte);

            mIndex_byte += aInSize_byte;
        }

    }
}
