
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON_Base.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/Base.h>

namespace KMS
{
    namespace JSON
    {
        // Public
        // //////////////////////////////////////////////////////////////////

        Base::~Base() {}

        // Protected
        // //////////////////////////////////////////////////////////////////

        Base::Base() {}

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Base& aD)
{
    // TODO
    
    return aOut;
}
