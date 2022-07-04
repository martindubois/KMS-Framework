
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
            if (NULL == lValue)
            {
                KMS_EXCEPTION_WITH_INFO(CONFIG_EXPAND, "getenv failed", aName);
            }

            unsigned int lResult = strlen(lValue);
            if ((0 >= lResult) || (aOutSize_byte <= lResult))
            {
                KMS_EXCEPTION_WITH_INFO(CONFIG_EXPAND, "Invalid environment variable value", aName);
            }

            strcpy(aOut, lValue);

            return lResult;
        }

    }
}
