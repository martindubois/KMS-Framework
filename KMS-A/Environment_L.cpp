
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
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
            assert(nullptr != aName);
            assert(nullptr != aOut);

            const auto lValue = getenv(aName);

            char lMsg[64 + LINE_LENGTH];

            sprintf_s(lMsg, "The \"%s\" environment variable does not exist", aName);
            KMS_EXCEPTION_ASSERT(nullptr != lValue, RESULT_EXPAND_FAILED, lMsg, "");

            auto lResult = strlen(lValue);

            sprintf_s(lMsg, "The value of \"%s\" is too long", aName);

            strcpy(aOut, lValue);

            return lResult;
        }

    }
}
