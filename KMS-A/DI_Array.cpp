
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Array.cpp

// TEST COVERAGE 2023-07-28 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Ptr_OF<KMS::DI::Object> EMPTY_ENTRY(nullptr, false);

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Array::AddEntry(Ptr_OF<Object>& aO)
        {
            mInternal.push_back(EMPTY_ENTRY);

            mInternal.back().Set(aO);
        }

        Object* Array::CreateEntry()
        {
            Ptr_OF<Object> lEntry(CallCreator(), true);

            Object* lResult = lEntry;

            AddEntry(lEntry);

            return lResult;
        }

        const Object* Array::GetEntry_R(int aIndex) const
        {
            assert(0 <= aIndex);

            if (static_cast<int>(mInternal.size()) <= aIndex)
            {
                return nullptr;
            }

            assert(nullptr != mInternal[aIndex]);

            return mInternal[aIndex];
        }

        Object* Array::GetEntry_RW(int aIndex)
        {
            assert(0 <= aIndex);

            if (static_cast<int>(mInternal.size()) <= aIndex)
            {
                return nullptr;
            }

            assert(nullptr != mInternal[aIndex]);

            return mInternal[aIndex];
        }

        void Array::RemoveEntry(int aIndex)
        {
            assert(0 <= aIndex);

            if (mInternal.size() <= static_cast<size_t>(aIndex))
            {
                char lMsg[64];
                sprintf_s(lMsg, "%d is not a valid index", aIndex);
                KMS_EXCEPTION(RESULT_INVALID_INDEX, lMsg, mInternal.size());
            }

            auto lIt = mInternal.begin() + aIndex;

            mInternal.erase(lIt);
        }

        void Array::SetEntry(int aIndex, Ptr_OF<Object>& aE)
        {
            assert(0 <= aIndex);

            if (mInternal.size() == aIndex)
            {
                AddEntry(aE);
            }
            else
            {
                if (mInternal.size() <= static_cast<size_t>(aIndex))
                {
                    char lMsg[64];
                    sprintf_s(lMsg, "%d is not a valid index", aIndex);
                    KMS_EXCEPTION(RESULT_INVALID_INDEX, lMsg, mInternal.size());
                }

                mInternal[aIndex].Set(aE);
            }
        }

        // ===== Container ==================================================

        unsigned int Array::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Array::IsEmpty() const { return mInternal.empty(); }

        Object* Array::FindObject_RW(const char* aName)
        {
            unsigned int lIndex;
            char         lRest[NAME_LENGTH];

            auto lRet = sscanf_s(aName, "%u.%[^ \n\r\t]", &lIndex, lRest SizeInfo(lRest));

            char lMsg[64 + NAME_LENGTH];

            if (1 > lRet)
            {
                sprintf_s(lMsg, "\"%s\" is not a valid name", aName);
                KMS_EXCEPTION(RESULT_INVALID_NAME, lMsg, lRet);
            }

            if (mInternal.size() < lIndex)
            {
                sprintf_s(lMsg, "%u is not a valid index", lIndex);
                KMS_EXCEPTION(RESULT_INVALID_INDEX, lMsg, mInternal.size());
            }

            Object* lResult;

            if (mInternal.size() == lIndex)
            {
                if (!IsDynamic())
                {
                    return nullptr;
                }

                // NOT TESTED
                lResult = CreateEntry();

                Send_OnChanged(lResult);
            }
            else
            {
                if (mInternal[lIndex].IsConst())
                {
                    sprintf_s(lMsg, "\"%s\" is read only (NOT TESTED)", aName);
                    KMS_EXCEPTION(RESULT_DENIED, lMsg, "");
                }
                lResult = mInternal[lIndex].Get();
            }

            if (2 == lRet)
            {
                // NOT TESTED
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

        void Array::Validate() const
        {
            for (auto& lEntry : mInternal)
            {
                auto lC = dynamic_cast<const DI::Container*>(lEntry.Get());
                if (nullptr != lC)
                {
                    lC->Validate();
                }
            }
        }

        // ===== Object =====================================================

        Array::~Array() { Clear(); }

        bool Array::Clear()
        {
            bool lResult = !mInternal.empty();

            for (auto& lEntry : mInternal)
            {
                lEntry.Release();
            }

            mInternal.clear();

            return lResult;
        }

    }
}
