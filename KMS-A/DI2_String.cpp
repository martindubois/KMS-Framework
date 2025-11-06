
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_String.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/String.h>

// Static fonction declarations
// //////////////////////////////////////////////////////////////////////////

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        String::String(unsigned int aSize_byte) : Sized(aSize_byte)
        {
            assert(2 <= aSize_byte);
        }

        // ===== IType ======================================================

        // "Value"
        void String::Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            assert(nullptr != aData);
            assert(nullptr != aOut);
            assert(4 <= aOutSize_byte);

            memset(aOut, 0, aOutSize_byte);

            auto lSize_byte = GetSize();
            assert(2 <= lSize_byte);

            auto lUsedSize_byte = strnlen(reinterpret_cast<const char*>(aData), lSize_byte);
            assert(lSize_byte >= lUsedSize_byte);

            KMS_EXCEPTION_ASSERT(aOutSize_byte - 2 > lUsedSize_byte, KMS::RESULT_OUTPUT_TOO_SHORT, "Output buffer too short", lUsedSize_byte);

            sprintf_s(aOut SizeInfoV(aOutSize_byte), "\"%s\"", reinterpret_cast<const char*>(aData));

            if (nullptr != aNext)
            {
                *aNext = aOut + lUsedSize_byte + 2;
            }
        }

        // "Value"
        void String::Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // Operators =
        // 
        // Value
        // "Value"
        // op Value
        // op "Value"
        void String::Decode_ASCII(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

        // "Value"
        void String::Decode_JSON(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

    }
}

using namespace KMS;

// Static fonctions
// //////////////////////////////////////////////////////////////////////////
