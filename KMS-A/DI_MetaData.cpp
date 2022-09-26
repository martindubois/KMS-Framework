
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_MetaData.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CopyString(const char** aInOut);

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const MetaData META_DATA_DELETE_OBJECT(NULL, NULL, MetaData::FLAG_DELETE_OBJECT);

        const unsigned int MetaData::FLAG_COPY_LABEL       = 0x00000001;
        const unsigned int MetaData::FLAG_COPY_NAME        = 0x00000002;
        const unsigned int MetaData::FLAG_DELETE_META_DATA = 0x00000004;
        const unsigned int MetaData::FLAG_DELETE_OBJECT    = 0x00000008;

        MetaData::MetaData(const char* aName, const char* aLabel, unsigned int aFlags) : mFlags(aFlags), mLabel(aLabel), mName(aName)
        {
            if (TestFlag(FLAG_COPY_LABEL))
            {
                CopyString(&mLabel);
            }

            if (TestFlag(FLAG_COPY_NAME))
            {
                CopyString(&mName);
            }
        }

        MetaData::~MetaData()
        {
            if ((NULL != mLabel) && TestFlag(FLAG_COPY_LABEL))
            {
                delete[] mLabel;
            }

            if ((NULL != mName) && TestFlag(FLAG_COPY_NAME))
            {
                delete[] mName;
            }
        }

        const char* MetaData::GetLabel() const { return mLabel; }
        const char* MetaData::GetName () const { return mName ; }

        unsigned int MetaData::GetName(char* aOut, unsigned int aOutSize_byte) const
        {
            if (NULL == mName)
            {
                KMS_EXCEPTION_ASSERT(2 <= aOutSize_byte, OUTPUT_TOO_SHORT, "The output buffer is too short");

                aOut[0] = '\0';
                return 0;
            }

            unsigned int lResult_byte = static_cast<unsigned int>(strlen(mName));
            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte + 2, OUTPUT_TOO_SHORT, "The output buffer is too short");

            strcpy_s(aOut SizeInfoV(aOutSize_byte), mName);
            return lResult_byte;
        }

        bool MetaData::IsLabeled() const { return NULL != mLabel; }
        bool MetaData::IsNamed  () const { return NULL != mName ; }

        // Internal
        // //////////////////////////////////////////////////////////////////

        bool MetaData::Is(const char* aName) const
        {
            assert(NULL != aName);

            if (NULL == mName)
            {
                return false;
            }

            return 0 == strcmp(mName, aName);
        }

        bool MetaData::TestFlag(unsigned int aFlag) const { return 0 != (mFlags & aFlag); }

    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void CopyString(const char** aInOut)
{
    assert(NULL != aInOut);

    const char* lIn = *aInOut;
    if (NULL != lIn)
    {
        unsigned int lLen = static_cast<unsigned int>(strlen(lIn)) + 2;

        char* lOut = new char[lLen];

        strcpy_s(lOut SizeInfoV(lLen), lIn);

        *aInOut = lOut;
    }
}
