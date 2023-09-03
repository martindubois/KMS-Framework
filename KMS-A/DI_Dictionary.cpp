
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Dictionary.cpp

// TEST COVERAGE  2023-07-28  KMS - Martin Dubois, P. Eng.

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
            assert(nullptr != aName);

            auto lIt = mInternal.find(aName);
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
            assert(nullptr != aName);

            auto lIt = mInternal.find(aName);
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
            auto lResult = CallCreator();
            assert(nullptr != lResult);

            AddEntry(aName, lResult, true, aMD);

            return lResult;
        }

        const Object* Dictionary::GetEntry_R(const char* aKey) const
        {
            assert(nullptr != aKey);

            auto lIt = mInternal.find(aKey);
            if (mInternal.end() == lIt)
            {
                return nullptr;
            }

            assert(nullptr != lIt->second);

            return lIt->second;
        }

        Object* Dictionary::GetEntry_RW(const char* aKey)
        {
            assert(nullptr != aKey);

            auto lIt = mInternal.find(aKey);
            if (mInternal.end() == lIt)
            {
                return nullptr;
            }

            assert(nullptr != lIt->second);

            return lIt->second;
        }

        // ===== Container ==================================================
        
        unsigned int Dictionary::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Dictionary::IsEmpty() const { return mInternal.empty(); }

        Object* Dictionary::FindObject_RW(const char* aName)
        {
            char lName[NAME_LENGTH];
            char lRest[NAME_LENGTH];

            auto lRet = sscanf_s(aName, "%[^.].%[^ \n\r\t]", lName SizeInfo(lName), lRest SizeInfo(lRest));

            char lMsg[64 + NAME_LENGTH];

            if (1 > lRet)
            {
                sprintf_s(lMsg, "\"%s\" is not a valid name", aName);
                KMS_EXCEPTION(RESULT_INVALID_NAME, lMsg, lRet);
            }

            Object* lResult;

            auto lIt = mInternal.find(lName);
            if (mInternal.end() == lIt)
            {
                if (!IsDynamic())
                {
                    return nullptr;
                }

                lResult = CreateEntry(lName);

                Send_OnChanged(lName);
            }
            else
            {
                if (lIt->second.IsConst())
                {
                    sprintf_s(lMsg, "\"%s\" is read only (NOT TESTED)", aName);
                    KMS_EXCEPTION(RESULT_DENIED, lMsg, "");
                }
                lResult = lIt->second.Get();
            }

            if (2 == lRet)
            {
                auto lContainer = dynamic_cast<DI::Container*>(lResult);

                if (nullptr == lContainer)
                {
                    sprintf_s(lMsg, "\"%s\" is not a valid name (NOT TESTED)", aName);
                    KMS_EXCEPTION(RESULT_INVALID_FORMAT, lMsg, lRet);
                }

                lResult = lContainer->FindObject_RW(lRest);
            }

            return lResult;
        }
        
        // ===== Object =====================================================
        Dictionary::~Dictionary() { Clear(); }

        bool Dictionary::Clear()
        {
            auto lResult = !mInternal.empty();

            for (auto& lVT : mInternal)
            {
                lVT.second.Release();
            }

            mInternal.clear();

            return lResult;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Dictionary::Entry::Entry(const Object* aObject, const MetaData* aMD)
            : Container::Entry(aObject), mMetaData(aMD)
        {}

        Dictionary::Entry::Entry(Object* aObject, bool aDelete, const MetaData* aMD)
            : Container::Entry(aObject, aDelete), mMetaData(aMD)
        {}

    }
}
