
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

        unsigned int Object::HTTP_Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(false);

            KMS_EXCEPTION(NOT_IMPLEMENTED, "The derived class does not implement the HTTP_Get method");
        }

        unsigned int Object::HTTP_Set(const char* aIn, unsigned int aInSize_byte)
        {
            assert(false);

            KMS_EXCEPTION(NOT_IMPLEMENTED, "The derived class does not implement the HTTP_Set method");
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Object::Object() {}

        void Object::VerifyOutputSize(unsigned int aMax, unsigned int aValue) const
        {
            KMS_EXCEPTION_ASSERT(aMax >= aValue + 1, OUTPUT_TOO_SHORT, "The output bufer is too small");
        }

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Object& aO)
{
    // TODO

    return aOut;
}
