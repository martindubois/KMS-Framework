
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Sized.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

namespace KMS
{
    namespace DI2
    {

        class Sized : public IType
        {

        public:

            Sized(unsigned int aSize_byte);

            // ===== IType ==================================================
            virtual unsigned int GetSize() const override;

        private:

            unsigned int mSize_byte;

        };

    }
}
