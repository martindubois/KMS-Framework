
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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

            unsigned int lResult = GetEnvironmentVariable(aName, aOut, aOutSize_byte);
            KMS_EXCEPTION_ASSERT((0 < lResult) && (aOutSize_byte > lResult), ENV_EXPAND_FAILED, "GetEnvironmentVariable failed", aName);

            return lResult;
        }

    }
}
