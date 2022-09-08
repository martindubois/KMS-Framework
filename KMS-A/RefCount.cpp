
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/RefCount.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/RefCount.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void RefCount::DecRefCount()
    {
        mRefCount--;

        if (0 == mRefCount)
        {
            delete this;
        }
    }

    void RefCount::IncRefCount() { mRefCount++; }

    // Protected
    // //////////////////////////////////////////////////////////////////////

    RefCount::RefCount() : mRefCount(1) {}

    RefCount::~RefCount() {}

}
