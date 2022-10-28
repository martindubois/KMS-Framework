
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Array.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Array::AddConstEntry(const Object* aO) { assert(NULL != aO); mInternal.push_back(const_cast<Object*>(aO)); }

        void Array::AddEntry(Object* aO, bool aDelete) { assert(NULL != aO); mInternal.push_back(Container::Entry(aO, aDelete)); }

        DI::Object* Array::CreateEntry()
        {
            DI::Object* lResult = CallCreator();

            AddEntry(lResult, true);

            return lResult;
        }

        const Object* Array::GetEntry_R(int aIndex) const
        {
            if (static_cast<int>(mInternal.size()) <= aIndex)
            {
                return NULL;
            }

            assert(NULL != mInternal[aIndex]);

            return mInternal[aIndex];
        }

        Object* Array::GetEntry_RW(int aIndex)
        {
            if (static_cast<int>(mInternal.size()) <= aIndex)
            {
                return NULL;
            }

            assert(NULL != mInternal[aIndex]);

            return mInternal[aIndex];
        }

        // ===== Container ==================================================

        unsigned int Array::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Array::IsEmpty() const { return mInternal.empty(); }

        Object* Array::FindObject_RW(const char* aName)
        {
            unsigned int lIndex;
            char         lRest[NAME_LENGTH];

            int lRet = sscanf_s(aName, "%u.%[^ \n\r\t]", &lIndex, &lRest SizeInfo(lRest));
            KMS_EXCEPTION_ASSERT(1 <= lRet, DI_NAME_INVALID, "Invalid name", aName);

            KMS_EXCEPTION_ASSERT(mInternal.size() >= lIndex, DI_INDEX_INVALID, "Invalid index", aName);

            Object* lResult;

            if (mInternal.size() == lIndex)
            {
                if (!IsDynamic())
                {
                    return NULL;
                }

                lResult = CreateEntry();
            }
            else
            {
                KMS_EXCEPTION_ASSERT(!mInternal[lIndex].IsConst(), DI_DENIED, "Denied", aName);
                lResult = mInternal[lIndex].Get();
            }

            if (2 == lRet)
            {
                DI::Container *lContainer = dynamic_cast<DI::Container*>(lResult);
                KMS_EXCEPTION_ASSERT(NULL != lContainer, DI_FORMAT_INVALID, "Invalid name", aName);

                lResult = lContainer->FindObject_RW(lRest);
            }

            return lResult;
        }

        // ===== Object =====================================================

        Array::~Array() { Clear(); }

        void Array::Clear()
        {
            for (Container::Entry& lEntry : mInternal)
            {
                lEntry.Release();
            }

            mInternal.clear();
        }

    }
}
