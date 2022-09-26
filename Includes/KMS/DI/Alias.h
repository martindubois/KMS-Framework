
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Alias.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Object.h>

namespace KMS
{
    namespace DI
    {

        class Alias : public Object
        {

        public:

            Alias(Object* aObj, const MetaData* aMD);

            Object* Get();

            // ===== Object =================================================
            ~Alias();

        private:

            Object* mInternal;

        };

    }
}
