
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/HTTP.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>

namespace KMS
{
    namespace HTTP
    {

        extern unsigned int Decode_Dictionary(DI::Dictionary* aDictionary, const char* aIn, unsigned int aInSize_byte);

        extern unsigned int Encode_Dictionary(const DI::Dictionary* aDictionary, char* aOut, unsigned int aOutSize_byte);

    }
}
