
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/JSON.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>

namespace KMS
{
    namespace JSON
    {

        extern unsigned int Decode(DI::Object*  aObject, const char* aIn , unsigned int aInSize_byte);
        extern unsigned int Decode(DI::Object** aObject, const char* aIn , unsigned int aInSize_byte);

        extern unsigned int Encode(const DI::Object* aObject, char* aOut, unsigned int aOutSize_byte);

        extern unsigned int Encode_Dictionary(const DI::Dictionary* aDictionary, char* aOut, unsigned int aOutSize_byte);

    }
}
