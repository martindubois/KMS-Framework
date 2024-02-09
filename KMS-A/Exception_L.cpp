
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Exception_L.cpp

#include "Component.h"

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void* Exception::RegisterTranslator() { return nullptr; }

    void Exception::RestoreTranslator(void*) {}

    void Exception::Construct_OSDep()
    {
        mLastError = errno;
    }

}
