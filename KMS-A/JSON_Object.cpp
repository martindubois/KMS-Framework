
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON_Object.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/Object.h>

namespace KMS
{
    namespace JSON
    {
        // Public
        // //////////////////////////////////////////////////////////////////

        Object::~Object() {}

        // Protected
        // //////////////////////////////////////////////////////////////////

        Object::Object() {}

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Object& aO)
{
    // TODO
    
    return aOut;
}
