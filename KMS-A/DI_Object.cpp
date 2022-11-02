
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Object.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
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
