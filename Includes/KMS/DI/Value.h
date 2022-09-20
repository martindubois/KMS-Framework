
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Value.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Object.h>

namespace KMS
{
    namespace DI
    {

        class MetaData;

        class Value : public Object
        {

        public:

            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const = 0;

            virtual void Set(const char* aIn) = 0;

            // ===== Object =================================================
            virtual ~Value();

        protected:

            Value(const MetaData* aMD);

        };

    }
}
