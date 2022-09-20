
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Object.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/Object.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Object::~Object() { ClearMetaData(); }

        const MetaData* Object::GetMetaData() const { return mMetaData; }

        void Object::SetMetaData(const MetaData* aMD) { ClearMetaData(); mMetaData = aMD; }

        unsigned int Object::GetName(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            if (NULL == mMetaData)
            {
                KMS_EXCEPTION_ASSERT(2 <= aOutSize_byte, OUTPUT_TOO_SHORT, "The output buffer is too short");

                aOut[0] = '\0';
            }
            else
            {
                lResult_byte = mMetaData->GetName(aOut, aOutSize_byte);
            }

            return lResult_byte;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        bool Object::Is(const char* aName) const { return (NULL != mMetaData) && mMetaData->Is(aName); }

        bool Object::TestFlag(unsigned int aFlags) const { return (NULL != mMetaData) && mMetaData->TestFlag(aFlags); }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Object::Object(const MetaData* aMD) : mMetaData(aMD) {}

        // Private
        // //////////////////////////////////////////////////////////////////

        void Object::ClearMetaData()
        {
            if (TestFlag(MetaData::FLAG_DELETE_META_DATA))
            {
                delete mMetaData;
            }

            mMetaData = NULL;
        }

    }
}
