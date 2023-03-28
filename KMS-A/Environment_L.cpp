
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

            char lMsg[64 + LINE_LENGTH];

            sprintf_s(lMsg, "The \"%s\" environment variable does not exist", aName);
            KMS_EXCEPTION_ASSERT(NULL != lValue, ENV_EXPAND_FAILED, lMsg, "");

            unsigned int lResult = strlen(lValue);

            sprintf_s(lMsg, "The value of \"%s\" is too long", aName);
            KMS_EXCEPTION_ASSERT((0 < lResult) && (aOutSize_byte > lResult), ENV_EXPAND_FAILED, lMsg, lResult);

            strcpy(aOut, lValue);

            return lResult;
        }

    }
}
