
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Int32.h
// Library   KMS-A

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace DI
    {

        class Int32 : public Value
        {

        public:

            static DI::Object* Create();

            Int32(int32_t aIn = 0);

            const Int32& operator = (int32_t aIn);

            operator int32_t () const;

            bool operator == (int32_t aIn);

            void Clear();

            void SetRadix(Radix aRadix);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Int32();

        private:

            int32_t mInternal;

            Radix mRadix;

        };

    }
}
