
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

        Object* Dictionary::Create() { return new Dictionary; }

        void Dictionary::AddConstEntry(const char* aName, const Object* aObject, const MetaData* aMD)
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
        
        unsigned int Dictionary::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Dictionary::IsEmpty() const { return mInternal.empty(); }

        Object* Dictionary::FindObject_RW(const char* aName)
        {
            char lName[NAME_LENGTH];
            char lRest[NAME_LENGTH];

            int lRet = sscanf_s(aName, "%[^.].%[^ \n\r\t]", lName SizeInfo(lName), &lRest SizeInfo(lRest));

            char lMsg[64 + NAME_LENGTH];

            sprintf_s(lMsg, "\"%s\" is not a valid name", aName);
            KMS_EXCEPTION_ASSERT(1 <= lRet, DI_NAME_INVALID, lMsg, lRet);

            Object* lResult;

            Internal::iterator lIt = mInternal.find(lName);
            if (mInternal.end() == lIt)
            {
                if (!IsDynamic())
                {
                    return NULL;
                }

                lResult = CreateEntry(lName);

                Send_OnChanged(lName);
            }
            else
            {
                sprintf_s(lMsg, "\"%s\" is read only", aName);
                KMS_EXCEPTION_ASSERT(!lIt->second.IsConst(), DI_DENIED, lMsg, "");
                lResult = lIt->second.Get();
            }

            if (2 == lRet)
            {
                DI::Container* lContainer = dynamic_cast<DI::Container*>(lResult);

                sprintf_s(lMsg, "\"%s\" is not a valid name", aName);
                KMS_EXCEPTION_ASSERT(NULL != lContainer, DI_FORMAT_INVALID, lMsg, lRet);

                lResult = lContainer->FindObject_RW(lRest);
            }

            return lResult;
        }
        
        // ===== Object =====================================================
        Dictionary::~Dictionary() { Clear(); }

        bool Dictionary::Clear()
        {
            bool lResult = !mInternal.empty();

            for (Internal::value_type lVT : mInternal)
            {
                lVT.second.Release();
            }

            mInternal.clear();

            return lResult;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Dictionary::Entry::Entry(const Object* aObject, const MetaData* aMD) : Container::Entry(aObject), mMetaData(aMD)
        {
        }

        Dictionary::Entry::Entry(Object* aObject, bool aDelete, const MetaData* aMD) : Container::Entry(aObject, aDelete), mMetaData(aMD)
        {
        }

    }
}
