
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Environment.h
// Status    PROD_READY (Windows)
// Library   KMS-A

#pragma once

namespace KMS
{
    namespace Env
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        // Exception  ENV_EXPAND_FAILED
        //            ENV_OUTPUT_TOO_SHORT
        void Expand(const char* aIn, char* aOut, unsigned int aOutSize_byte);

        // Exception  ENV_EXPAND_FAILED
        unsigned int  GetVariableValue(const char* aName, char* aOut, unsigned int aOutSize_byte);

    }
}
