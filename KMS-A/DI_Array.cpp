
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

        void Array::Clear()
        {
            for (Container::Entry& lEntry : mInternal)
            {
                lEntry.Release();
            }

            mInternal.clear();
        }

        unsigned int Array::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Array::IsEmpty() const { return mInternal.empty(); }

        void Array::AddEntry(Object* aO, bool aDelete) { assert(NULL != aO); mInternal.push_back(Container::Entry(aO, aDelete)); }

        void Array::AddEntry(const Object* aO) { assert(NULL != aO); mInternal.push_back(const_cast<Object*>(aO)); }

        DI::Object* Array::CreateEntry()
        {
            DI::Object* lResult = CallCreator();

            AddEntry(lResult);

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

        // ===== Object =====================================================

        Array::~Array() { Clear(); }

    }
}
