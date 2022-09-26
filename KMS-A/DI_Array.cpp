
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Array.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>

#include <KMS/DI/Array.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Array::Array(const MetaData* aMD) : Object(aMD), mCreator(NULL) {}

        Object* Array::operator [] (int aIndex)
        {
            if (mInternal.size() <= aIndex)
            {
                return NULL;
            }

            assert(NULL != mInternal[aIndex]);

            return mInternal[aIndex];
        }

        const Object* Array::operator [] (int aIndex) const
        {
            if (mInternal.size() <= aIndex)
            {
                return NULL;
            }

            assert(NULL != mInternal[aIndex]);

            return mInternal[aIndex];
        }

        Object* Array::operator [] (const char* aKey)
        {
            assert(NULL != aKey);

            for (Object* lObject : mInternal)
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

            for (Object* lObject : mInternal)
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
            for (Object* lO : mInternal)
            {
                assert(NULL != lO);

                lO->Release();
            }

            mInternal.clear();
        }

        unsigned int Array::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Array::IsEmpty() const { return mInternal.empty(); }

        void Array::SetCreator(Creator aC) { mCreator = aC; }

        void Array::AddEntry(Object* aO) { assert(NULL != aO); mInternal.push_back(aO); }

        void Array::AddEntry(const Object* aO) { assert(NULL != aO); mInternal.push_back(const_cast<Object*>(aO)); }

        DI::Object* Array::CreateEntry(const DI::MetaData* aMD)
        {
            DI::Object* lResult = NULL;

            if (NULL != mCreator)
            {
                if (NULL == aMD)
                {
                    lResult = mCreator(&META_DATA_DELETE_OBJECT);
                }
                else
                {
                    lResult = mCreator(aMD);
                }

                AddEntry(lResult);
            }

            return lResult;
        }

        DI::Object* Array::CreateEntry(const char* aName, const char* aLabel, unsigned int aFlags)
        {
            DI::Object* lResult = NULL;

            if (NULL != mCreator)
            {
                unsigned int lFlags = aFlags | MetaData::FLAG_DELETE_META_DATA | MetaData::FLAG_DELETE_OBJECT;

                lResult = mCreator(new MetaData(aName, aLabel, lFlags));

                AddEntry(lResult);
            }

            return lResult;
        }

        // ===== Object =====================================================

        Array::~Array() { Clear(); }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const Array::Internal& Array::GetInternal() const { return mInternal; }

        Array::Internal& Array::GetInternal() { return mInternal; }

    }
}
