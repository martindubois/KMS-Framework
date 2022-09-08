
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON_Array.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/JSON.h>

#include <KMS/JSON/Array.h>

namespace KMS
{
    namespace JSON
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Array::Array() {}

        void Array::operator += (Array* aA)
        {
            Internal* lA = aA->GetInternal();

            for (Object* lObj : *lA)
            {
                assert(NULL != lObj);

                lObj->IncRefCount();

                *this += lObj;
            }
        }

        void Array::operator += (Object* aE) { assert(NULL != aE); mEntries.push_back(aE); }

        // ===== Object =====================================================

        Array::~Array() { Clear(); }

        void Array::Clear()
        {
            for (Object* lObj : mEntries)
            {
                assert(NULL != lObj);

                lObj->DecRefCount();
            }

            mEntries.clear();
        }

        bool Array::IsEmpty() const { return mEntries.empty(); }

        unsigned int Array::JSON_Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            for (const Object* lObj : mEntries)
            {
                assert(NULL != lObj);

                if (aOutSize_byte < lResult_byte + 1)
                {
                    KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output bufer is too small");
                }

                aOut[lResult_byte] = (0 == lResult_byte) ? '[' : ','; lResult_byte++;

                lResult_byte += lObj->JSON_Get(aOut + lResult_byte, aOutSize_byte - lResult_byte);
            }

            if (aOutSize_byte < lResult_byte + 2)
            {
                KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output bufer is too small");
            }

            aOut[lResult_byte] = ']'; lResult_byte++;
            aOut[lResult_byte] = '\0';

            return lResult_byte;
        }

        unsigned int Array::JSON_Set(const char* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);
            
            unsigned int lResult_byte = 0;
            while (aInSize_byte < lResult_byte)
            {
                switch (aIn[lResult_byte])
                {
                case ' ':
                case '\n':
                case '\r':
                case '\t':
                case '[': break;

                case ']': return lResult_byte;

                default:
                    Object* lObj;

                    lResult_byte += CreateFromJSON(aIn + lResult_byte, aInSize_byte - lResult_byte, &lObj);
                    assert(NULL != lObj);

                    *this += lObj;
                }
            }

            return lResult_byte;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const Array::Internal& Array::GetInternal() const { return mEntries; }

        Array::Internal* Array::GetInternal() { return &mEntries; }

    }
}
