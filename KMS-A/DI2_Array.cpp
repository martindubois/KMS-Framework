
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Array.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Array.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Array::Array(unsigned int aCount, const IType* aElementType)
            : Sized(aCount * aElementType->GetSize())
            , mElementType(aElementType)
        {
            assert(nullptr != aElementType);
        }

        // ===== IType ======================================================

        // { Value0; Value1; }
        void Array::Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // [ Value0, Value1 ]
        void Array::Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // [0] op Value0
        // { Value0; Value1; }
        // = { Value0; Value1; }
        void Array::Decode_ASCII(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

        // [ Value0, Value1 ]
        void Array::Decode_JSON(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

    }
}
