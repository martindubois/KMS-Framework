
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Alias.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Alias.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Alias::Alias(Object* aObj, const MetaData* aMD) : Object(aMD), mInternal(aObj) {}

        Object* Alias::Get() { return mInternal; }

        // ===== Object =====================================================
        Alias::~Alias() {}

    }
}
