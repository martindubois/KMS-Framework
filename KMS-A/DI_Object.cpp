
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Object.cpp

// TEST COVERAGE  2023-07-28  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Object.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Object::~Object() {}

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Object::Send_OnChanged(void* aData) { mOnChanged.Send(this, aData); }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Object::Object() {}

    }
}
