
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Array_Sparse.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>

#include <KMS/DI/Array_Sparse.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Array_Sparse::Array_Sparse(const MetaData* aMD) : Object(aMD), mCreator(NULL) {}

        Object* Array_Sparse::operator [] (int aIndex)
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

        void Array_Sparse::Clear()
        {
            for (Internal::value_type lVT : mInternal)
            {
                Object* lObject = lVT.second;
                assert(NULL != lObject);

                lObject->Release();
            }

            mInternal.clear();
        }

        void Array_Sparse::SetCreator(Creator aC) { mCreator = aC; }

        void Array_Sparse::AddEntry(int aIndex, Object* aO)
        {
            assert(NULL != aO);

            Internal::iterator lIt = mInternal.find(aIndex);
            if (mInternal.end() == lIt)
            {
                mInternal.insert(Internal::value_type(aIndex, aO));
            }
            else
            {
                DI::Object* lObject = lIt->second;
                assert(NULL != lObject);

                lObject->Release();

                lIt->second = aO;
            }
        }

        DI::Object* Array_Sparse::CreateEntry(int aIndex, const MetaData* aMD)
        {
            DI::Object* lResult = mCreator(aMD);
            assert(NULL != lResult);

            AddEntry(aIndex, lResult);

            return lResult;
        }

        // ===== Object =====================================================

        Array_Sparse::~Array_Sparse() { Clear(); }

    }
}
