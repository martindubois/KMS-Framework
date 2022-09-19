
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

        void Container::operator += (Object* aO) { assert(NULL != aO); mObjects.push_back(aO); }

        Object* Container::operator [] (int aIndex)
        {
            if (mObjects.size() <= aIndex)
            {
                return NULL;
            }

            assert(NULL != mObjects[aIndex]);

            return mObjects[aIndex];
        }

        const Object* Container::operator [] (int aIndex) const
        {
            if (mObjects.size() <= aIndex)
            {
                return NULL;
            }

            assert(NULL != mObjects[aIndex]);

            return mObjects[aIndex];
        }

        Object* Container::operator [] (const char* aKey)
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

        const Object* Container::operator [] (const char* aKey) const
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

        unsigned int Container::GetCount() const { return static_cast<unsigned int>(mObjects.size()); }

        // ===== Object =====================================================

        Container::~Container()
        {
            for (Object* lO : mObjects)
            {
                assert(NULL != lO);

                const MetaData* lMD = lO->GetMetaData();
                if ((NULL != lMD) && lMD->TestFlag(MetaData::FLAG_DELETE_OBJECT))
                {
                    delete lO;
                }
            }
        }

        Array::~Array() {}

        Dictionary::~Dictionary() {}

    }
}
