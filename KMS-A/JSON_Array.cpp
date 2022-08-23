
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

        Array::Array(const Array& aA) { *this += aA; }

        const Array& Array::operator = (const Array& aA)
        {
            Clear();

            *this += aA;

            return *this;
        }

        void Array::operator += (const Array& aA)
        {
            const Internal& lA = aA.GetInternal();

            for (const Base* lB : lA)
            {
                assert(NULL != lB);

                *this += lB->Copy();
            }
        }

        void Array::operator += (Base* aE) { assert(NULL != aE); mEntries.push_back(aE); }

        // ===== Base =======================================================

        Array::~Array() { Clear(); }

        void Array::Clear()
        {
            for (Base* lBase : mEntries)
            {
                assert(NULL != lBase);

                delete lBase;
            }

            mEntries.clear();
        }

        Base* Array::Copy() const { return new Array(*this); }

        bool Array::IsEmpty() const { return mEntries.empty(); }

        unsigned int Array::HTTP_Get(char* aOut, unsigned int aOutSize_byte) const
        {
            KMS_EXCEPTION(TODO, "Not implemented yet");
        }

        unsigned int Array::HTTP_Set(const char* aIn, unsigned int aInSize_byte)
        {
            KMS_EXCEPTION(TODO, "Not implemented yet");
        }

        unsigned int Array::JSON_Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            for (const Base* lBase : mEntries)
            {
                assert(NULL != lBase);

                if (aOutSize_byte < lResult_byte + 1)
                {
                    KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output bufer is too small");
                }

                aOut[lResult_byte] = (0 == lResult_byte) ? '[' : ','; lResult_byte++;

                lResult_byte += lBase->JSON_Get(aOut + lResult_byte, aOutSize_byte - lResult_byte);
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
                    Base* lBase;

                    lResult_byte += CreateFromJSON(aIn + lResult_byte, aInSize_byte - lResult_byte, &lBase);
                    assert(NULL != lBase);

                    *this += lBase;
                }
            }

            return lResult_byte;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const Array::Internal& Array::GetInternal() const { return mEntries; }

    }
}
