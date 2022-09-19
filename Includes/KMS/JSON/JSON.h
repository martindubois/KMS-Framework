
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/JSON.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>

namespace KMS
{
    namespace JSON
    {

        class Object;

        unsigned int CreateFromHTTP(const char* aIn, unsigned int aInSize_byte, Object** aOut);
        unsigned int CreateFromJSON(const char* aIn, unsigned int aInSize_byte, Object** aOut);

        extern unsigned int Decode(DI::Object*  aObject, const char* aIn , unsigned int aInSize_byte);
        extern unsigned int Decode(DI::Object** aObject, const char* aIn , unsigned int aInSize_byte);

        extern unsigned int Decode_Dictionary(DI::Dictionary* aDictionary, const char* aIn, unsigned int aInSize_byte);

        extern unsigned int Encode(const DI::Object* aObject, char* aOut, unsigned int aOutSize_byte);

        extern unsigned int Encode_Dictionary(DI::Dictionary* aDictionary, char* aOut, unsigned int aOutSize_byte);

    }
}
