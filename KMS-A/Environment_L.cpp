
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Environment_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Environment.h>

namespace KMS
{
    namespace Env
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        unsigned int GetVariableValue(const char* aName, char* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aName);
            assert(NULL != aOut);

            const char* lValue = getenv(aName);
            KMS_EXCEPTION_ASSERT(NULL != lValue, ENV_EXPAND_FAILED, "getenv failed", aName);

            unsigned int lResult = strlen(lValue);
            KMS_EXCEPTION_ASSERT((0 < lResult) && (aOutSize_byte > lResult), ENV_EXPAND_FAILED, "Invalid environment variable value", aName);

            strcpy(aOut, lValue);

            return lResult;
        }

    }
}
