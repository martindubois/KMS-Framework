
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Dictionary.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Object* Dictionary::Create() { return new Dictionary(); }

        void Dictionary::AddEntry(const char* aName, const Object* aObject, const MetaData* aMD)
        {
            assert(NULL != aName);

            Internal::iterator lIt = mInternal.find(aName);
            if (mInternal.end() == lIt)
            {
                mInternal.insert(Internal::value_type(aName, Entry(const_cast<Object*>(aObject), aMD)));
            }
            else
            {
                lIt->second.mMetaData = aMD;
                lIt->second.Set(aObject);
            }
        }

        void Dictionary::AddEntry(const char* aName, Object* aObject, bool aDelete, const MetaData* aMD)
        {
            assert(NULL != aName);

            Internal::iterator lIt = mInternal.find(aName);
            if (mInternal.end() == lIt)
            {
                mInternal.insert(Internal::value_type(aName, Entry(aObject, aDelete, aMD)));
            }
            else
            {
                lIt->second.mMetaData = aMD;
                lIt->second.Set(aObject, aDelete);
            }
        }

        Object* Dictionary::CreateEntry(const char* aName, const MetaData* aMD)
        {
            Object* lResult = CallCreator();
            assert(NULL != lResult);

            AddEntry(aName, lResult, true, aMD);

            return lResult;
        }

        const Object* Dictionary::GetEntry_R(const char* aKey) const
        {
            assert(NULL != aKey);

            Internal::const_iterator lIt = mInternal.find(aKey);
            if (mInternal.end() == lIt)
            {
                return NULL;
            }

            assert(NULL != lIt->second);

            return lIt->second;
        }

        Object* Dictionary::GetEntry_RW(const char* aKey)
        {
            assert(NULL != aKey);

            Internal::iterator lIt = mInternal.find(aKey);
            if (mInternal.end() == lIt)
            {
                return NULL;
            }

            assert(NULL != lIt->second);

            return lIt->second;
        }

        // ===== Container ==================================================
        
        void Dictionary::Clear()
        {
            for (Internal::value_type lVT : mInternal)
            {
                lVT.second.Release();
            }

            mInternal.clear();
        }

        unsigned int Dictionary::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Dictionary::IsEmpty() const { return mInternal.empty(); }
        
        // ===== Object =====================================================
        Dictionary::~Dictionary() { Clear(); }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const Dictionary::Internal& Dictionary::GetInternal() const { return mInternal; }

        Dictionary::Internal& Dictionary::GetInternal() { return mInternal; }

        Dictionary::Entry::Entry(const Object* aObject, const MetaData* aMD) : Container::Entry(aObject), mMetaData(aMD)
        {
        }

        Dictionary::Entry::Entry(Object* aObject, bool aDelete, const MetaData* aMD) : Container::Entry(aObject, aDelete), mMetaData(aMD)
        {
        }

    }
}
