
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Environment.h

#pragma once

namespace KMS
{
    namespace Env
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        void Expand(const char* aIn, char* aOut, unsigned int aOutSize_byte);

        unsigned int  GetVariableValue(const char* aName, char* aOut, unsigned int aOutSize_byte);

    }
}
