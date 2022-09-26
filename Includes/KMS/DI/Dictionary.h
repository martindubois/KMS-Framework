
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Array.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

namespace KMS
{
    namespace DI
    {

        class Dictionary : public Array
        {

        public:

            Dictionary(const MetaData* aMD);

            // ===== Object =================================================
            virtual ~Dictionary();

        };

    }
}
