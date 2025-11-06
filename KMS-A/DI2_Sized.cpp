
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Sized.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Sized.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Sized::Sized(unsigned int aSize_byte) : mSize_byte(aSize_byte)
        {
            assert(0 < aSize_byte);
        }

        // ===== IType ======================================================

        unsigned int Sized::GetSize() const
        {
            assert(0 < mSize_byte);

            return mSize_byte;
        }

    }
}
