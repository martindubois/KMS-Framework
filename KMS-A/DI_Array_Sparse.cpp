
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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
            assert(nullptr != aO);

            auto lIt = mInternal.find(aIndex);
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
            assert(nullptr != aO);

            auto lIt = mInternal.find(aIndex);
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
            auto lResult = CallCreator();
            assert(nullptr != lResult);

            AddEntry(aIndex, lResult, true);

            return lResult;
        }

        const Object* Array_Sparse::GetEntry_R(int aIndex) const
        {
            const Object* lResult = nullptr;

            auto lIt = mInternal.find(aIndex);
            if (mInternal.end() != lIt)
            {
                lResult = lIt->second;
                assert(nullptr != lResult);
            }

            return lResult;
        }

        Object* Array_Sparse::GetEntry_RW(int aIndex)
        {
            Object* lResult = nullptr;

            auto lIt = mInternal.find(aIndex);
            if (mInternal.end() != lIt)
            {
                lResult = lIt->second;
                assert(nullptr != lResult);
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

            auto lRet = sscanf_s(aName, "%u.%[^ \n\r\t]", &lIndex, lRest SizeInfo(lRest));

            char lMsg[64 + NAME_LENGTH];

            sprintf_s(lMsg, "\"%s\" is not a valid element name", aName);
            KMS_EXCEPTION_ASSERT(1 <= lRet, DI_NAME_INVALID, lMsg, lRet);

            Object * lResult;

            auto lIt = mInternal.find(lIndex);
            if (mInternal.end() == lIt)
            {
                if (!IsDynamic())
                {
                    return nullptr;
                }

                lResult = CreateEntry(lIndex);

                Send_OnChanged(lResult);
            }
            else
            {
                sprintf_s(lMsg, "\"%s\" is read only", aName);
                KMS_EXCEPTION_ASSERT(!lIt->second.IsConst(), DI_DENIED, lMsg, "");

                lResult = lIt->second.Get();
            }

            if (2 == lRet)
            {
                auto lContainer = dynamic_cast<DI::Container*>(lResult);
                KMS_EXCEPTION_ASSERT(nullptr != lContainer, DI_FORMAT_INVALID, "Invalid name", aName);

                lResult = lContainer->FindObject_RW(lRest);
            }

            return lResult;
        }

        // ===== Object =====================================================

        Array_Sparse::~Array_Sparse() { Clear(); }

        bool Array_Sparse::Clear()
        {
            auto lResult = !mInternal.empty();

            for (auto& lVT : mInternal)
            {
                lVT.second.Release();
            }

            mInternal.clear();

            return lResult;
        }

    }
}
