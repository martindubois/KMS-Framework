
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Float.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Float.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Float::Float(unsigned int aSize_byte) : Sized(aSize_byte)
        {
            assert((sizeof(float) == aSize_byte) || (sizeof(double) == aSize_byte));
        }

        // ===== IType ======================================================

        // Value
        void Float::Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // Value
        void Float::Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // Operators = += -= *= /=
        // 
        // Value
        // op Value
        void Float::Decode_ASCII(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

        // Value
        void Float::Decode_JSON(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

    }
}
