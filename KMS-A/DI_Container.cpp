
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Container.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>

#include <KMS/DI/Container.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Array::Array(const MetaData* aMD) : Object(aMD) {}

        Dictionary::Dictionary(const MetaData* aMD) : Array(aMD) {}

        void Array::operator += (Object* aO) { assert(NULL != aO); mObjects.push_back(aO); }

        Object* Array::operator [] (int aIndex)
        {
            if (mObjects.size() <= aIndex)
            {
                return NULL;
            }

            assert(NULL != mObjects[aIndex]);

            return mObjects[aIndex];
        }

        const Object* Array::operator [] (int aIndex) const
        {
            if (mObjects.size() <= aIndex)
            {
                return NULL;
            }

            assert(NULL != mObjects[aIndex]);

            return mObjects[aIndex];
        }

        Object* Array::operator [] (const char* aKey)
        {
            assert(NULL != aKey);

            for (Object* lObject : mObjects)
            {
                assert(NULL != lObject);

                if (lObject->Is(aKey))
                {
                    return lObject;
                }
            }

            return NULL;
        }

        const Object* Array::operator [] (const char* aKey) const
        {
            assert(NULL != aKey);

            for (Object* lObject : mObjects)
            {
                assert(NULL != lObject);

                if (lObject->Is(aKey))
                {
                    return lObject;
                }
            }

            return NULL;
        }

        void Array::Clear()
        {
            for (Object* lO : mObjects)
            {
                assert(NULL != lO);

                if (lO->TestFlag(MetaData::FLAG_DELETE_OBJECT))
                {
                    delete lO;
                }
            }

            mObjects.clear();
        }

        unsigned int Array::GetCount() const { return static_cast<unsigned int>(mObjects.size()); }

        bool Array::IsEmpty() const { return mObjects.empty(); }

        // ===== Object =====================================================

        Array::~Array() { Clear(); }

        Dictionary::~Dictionary() {}

    }
}
