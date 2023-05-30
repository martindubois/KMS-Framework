
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Environment_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

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

            auto lResult = GetEnvironmentVariable(aName, aOut, aOutSize_byte);

            char lMsg[64 + NAME_LENGTH];
            sprintf_s(lMsg, "Cannot retrieve the value of \"%s\"", aName);
            KMS_EXCEPTION_ASSERT((0 < lResult) && (aOutSize_byte > lResult), ENV_EXPAND_FAILED, lMsg, lResult);

            return lResult;
        }

    }
}
