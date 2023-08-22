
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Array.cpp

// TEST COVERAGE  2023-07-28  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Array::AddConstEntry(const Object* aO) { assert(nullptr != aO); mInternal.push_back(Container::Entry(aO)); }

        void Array::AddEntry(Object* aO, bool aDelete) { assert(nullptr != aO); mInternal.push_back(Container::Entry(aO, aDelete)); }

        DI::Object* Array::CreateEntry()
        {
            auto lResult = CallCreator();

            AddEntry(lResult, true);

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
                KMS_EXCEPTION(DI_INDEX_INVALID, lMsg, mInternal.size());
            }

            auto lIt = mInternal.begin() + aIndex;

            mInternal.erase(lIt);
        }

        void Array::SetEntry(int aIndex, Object* aE, bool aDelete)
        {
            assert(0 <= aIndex);

            if (mInternal.size() == aIndex)
            {
                AddEntry(aE, aDelete);
            }
            else
            {
                if (mInternal.size() <= static_cast<size_t>(aIndex))
                {
                    char lMsg[64];
                    sprintf_s(lMsg, "%d is not a valid index", aIndex);
                    KMS_EXCEPTION(DI_INDEX_INVALID, lMsg, mInternal.size());
                }

                mInternal[aIndex].Set(aE, aDelete);
            }
        }

        // ===== Container ==================================================

        unsigned int Array::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Array::IsEmpty() const { return mInternal.empty(); }

        Object* Array::FindObject_RW(const char* aName)
        {
            unsigned int lIndex;
            char         lRest[NAME_LENGTH];

            auto lRet = sscanf_s(aName, "%u.%[^ \n\r\t]", &lIndex, &lRest SizeInfo(lRest));

            char lMsg[64 + NAME_LENGTH];

            if (1 > lRet)
            {
                sprintf_s(lMsg, "\"%s\" is not a valid name", aName);
                KMS_EXCEPTION(DI_NAME_INVALID, lMsg, lRet);
            }

            if (mInternal.size() < lIndex)
            {
                sprintf_s(lMsg, "%u is not a valid index", lIndex);
                KMS_EXCEPTION(DI_INDEX_INVALID, lMsg, mInternal.size());
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
                    KMS_EXCEPTION(DI_DENIED, lMsg, "");
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
                    KMS_EXCEPTION(DI_FORMAT_INVALID, lMsg, lRet);
                }

                lResult = lContainer->FindObject_RW(lRest);
            }

            return lResult;
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
