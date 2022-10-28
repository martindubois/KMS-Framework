
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Array_Sparse.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array_Sparse.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Array_Sparse::AddConstEntry(int aIndex, const Object* aO)
        {
            assert(NULL != aO);

            Internal::iterator lIt = mInternal.find(aIndex);
            if (mInternal.end() == lIt)
            {
                mInternal.insert(Internal::value_type(aIndex, Entry(aO)));
            }
            else
            {
                lIt->second.Set(aO);
            }
        }

        void Array_Sparse::AddEntry(int aIndex, Object* aO, bool aDelete)
        {
            assert(NULL != aO);

            Internal::iterator lIt = mInternal.find(aIndex);
            if (mInternal.end() == lIt)
            {
                mInternal.insert(Internal::value_type(aIndex, Entry(aO, aDelete)));
            }
            else
            {
                lIt->second.Set(aO, aDelete);
            }
        }

        DI::Object* Array_Sparse::CreateEntry(int aIndex)
        {
            DI::Object* lResult = CallCreator();
            assert(NULL != lResult);

            AddEntry(aIndex, lResult, true);

            return lResult;
        }

        const Object* Array_Sparse::GetEntry_R(int aIndex) const
        {
            const Object* lResult = NULL;

            Internal::const_iterator lIt = mInternal.find(aIndex);
            if (mInternal.end() != lIt)
            {
                lResult = lIt->second;
                assert(NULL != lResult);
            }

            return lResult;
        }

        Object* Array_Sparse::GetEntry_RW(int aIndex)
        {
            Object* lResult = NULL;

            Internal::iterator lIt = mInternal.find(aIndex);
            if (mInternal.end() != lIt)
            {
                lResult = lIt->second;
                assert(NULL != lResult);
            }

            return lResult;
        }

        // ===== Container ==================================================

        unsigned int Array_Sparse::GetCount() const { return static_cast<unsigned int>(mInternal.size()); }

        bool Array_Sparse::IsEmpty() const { return mInternal.empty(); }

        Object* Array_Sparse::FindObject_RW(const char* aName)
        {
            unsigned int lIndex;
            char         lRest[NAME_LENGTH];

            int lRet = sscanf_s(aName, "%u.%[^ \n\r\t]", &lIndex, &lRest SizeInfo(lRest));
            KMS_EXCEPTION_ASSERT(1 <= lRet, DI_NAME_INVALID, "Invalid name", aName);

            Object * lResult;

            Internal::iterator lIt = mInternal.find(lIndex);
            if (mInternal.end() == lIt)
            {
                if (!IsDynamic())
                {
                    return NULL;
                }

                lResult = CreateEntry(lIndex);
            }
            else
            {
                KMS_EXCEPTION_ASSERT(!lIt->second.IsConst(), DI_DENIED, "Denied", aName);
                lResult = lIt->second.Get();
            }

            if (2 == lRet)
            {
                DI::Container* lContainer = dynamic_cast<DI::Container*>(lResult);
                KMS_EXCEPTION_ASSERT(NULL != lContainer, DI_FORMAT_INVALID, "Invalid name", aName);

                lResult = lContainer->FindObject_RW(lRest);
            }

            return lResult;
        }

        // ===== Object =====================================================

        Array_Sparse::~Array_Sparse() { Clear(); }

        void Array_Sparse::Clear()
        {
            for (Internal::value_type lVT : mInternal)
            {
                lVT.second.Release();
            }

            mInternal.clear();
        }

    }
}
