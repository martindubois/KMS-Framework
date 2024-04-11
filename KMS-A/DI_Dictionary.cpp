
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Dictionary.cpp

// TEST COVERAGE 2023-07-28 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI::Dictionary::Entry EMPTY_ENTRY;

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Object* Dictionary::Create() { return new Dictionary; }

        Dictionary::Dictionary() {}

        void Dictionary::operator += (const Dictionary& aIn)
        {
            for (auto& lEntry : aIn.mInternal)
            {
                Ptr_OF<DI::Object> lNewEntry(lEntry.second.Get());

                AddEntry(lEntry.first.c_str(), lNewEntry);
            }
        }

        void Dictionary::AddEntry(const char* aName, Ptr_OF<Object>& aObject, const MetaData* aMD)
        {
            assert(nullptr != aName);

            auto lIt = mInternal.find(aName);
            if (mInternal.end() == lIt)
            {
                auto lRet = mInternal.insert(Internal::value_type(aName, EMPTY_ENTRY));
                lRet.first->second.Set(aObject, aMD);
            }
            else
            {
                lIt->second.Set(aObject, aMD);
            }
        }

        Object* Dictionary::CreateEntry(const char* aName, const MetaData* aMD)
        {
            Ptr_OF<Object> lEntry(CallCreator(), true);

            Object* lResult = lEntry;

            AddEntry(aName, lEntry, aMD);

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

        void Dictionary::Validate() const
        {
            for (auto& lVT : mInternal)
            {
                auto lC = dynamic_cast<const DI::Container*>(lVT.second.Get());
                if (nullptr != lC)
                {
                    lC->Validate();
                }
            }
        }
        
        // ===== Object =====================================================
        Dictionary::~Dictionary() { Clear(); }

        bool Dictionary::Clear()
        {
            auto lResult = !mInternal.empty();

            mInternal.clear();

            return lResult;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Dictionary::Entry::Entry() : mMetaData(nullptr) {}

        void Dictionary::Entry::Set(Ptr_OF<Object>& aObject, const MetaData* aMD)
        {
            Ptr_OF<Object>::Set(aObject);

            mMetaData = aMD;
        }

    }
}
