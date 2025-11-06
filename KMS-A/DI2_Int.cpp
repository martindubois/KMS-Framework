
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Int.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Int.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Int::Int(unsigned int aSize_byte) : Sized(aSize_byte)
        {
            assert((sizeof(int8_t ) == aSize_byte)
                || (sizeof(int16_t) == aSize_byte)
                || (sizeof(int32_t) == aSize_byte)
                || (sizeof(int64_t) == aSize_byte));
        }

        // ===== IType ======================================================

        // Value
        void Int::Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // Value
        void Int::Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // Operators += -= *= /= |= &= ^=
        // 
        // Value
        // op Value
        void Int::Decode_ASCII(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

        // Value
        void Int::Decode_JSON(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

    }
}
