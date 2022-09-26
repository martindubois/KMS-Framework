
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Boolean.h

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

            static DI::Object* Create(KMS::DI::MetaData* aMD);

            Boolean(bool aIn, const MetaData* aMD);

            void operator = (bool aIn);

            operator bool () const;

            void Clear();

            void Set(bool aIn = true);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Boolean();

        private:

            bool mInternal;

        };

    }
}
