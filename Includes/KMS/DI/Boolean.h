
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Boolean.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>

namespace KMS
{
    namespace DI
    {

        class Boolean : public Value
        {

        public:

            static DI::Object* Create();

            Boolean(bool aIn = false);

            void operator = (bool aIn);

            operator bool () const;

            bool Get() const;

            void Set(bool aIn = true);

            // ===== Value ==================================================
            virtual bool         Clear();
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Boolean();

        // Internal

            bool mInternal;

        };

    }
}
