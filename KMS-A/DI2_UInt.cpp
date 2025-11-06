
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_UInt.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/UInt.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        UInt::UInt(unsigned int aSize_byte) : Sized(aSize_byte)
        {
            assert((sizeof(uint8_t ) == aSize_byte)
                || (sizeof(uint16_t) == aSize_byte)
                || (sizeof(uint32_t) == aSize_byte)
                || (sizeof(uint64_t) == aSize_byte));
        }

        // ===== IType ======================================================

        // Value
        void UInt::Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // Value
        void UInt::Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // Operators = += -= *= /= |= &= ^=
        // 
        // Value
        // op Value
        void UInt::Decode_ASCII(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

        // Value
        void UInt::Decode_JSON(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

    }
}
