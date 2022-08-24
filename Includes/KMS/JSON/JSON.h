
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/Dictionary.h

#pragma once

namespace KMS
{
    namespace JSON
    {

        class Object;

        unsigned int CreateFromHTTP(const char* aIn, unsigned int aInSize_byte, Object** aOut);
        unsigned int CreateFromJSON(const char* aIn, unsigned int aInSize_byte, Object** aOut);

    }
}
