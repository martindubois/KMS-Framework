
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/UInt8.h
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

        class UInt8 : public Value
        {

        public:

            static DI::Object* Create();

            UInt8(uint8_t aIn = 0);

            void operator = (uint8_t aIn);

            operator uint8_t () const;

            bool operator == (uint8_t aIn);

            void Clear();

            void SetRadix(Radix aRadix);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~UInt8();

        private:

            uint8_t mInternal;

            Radix mRadix;

        };

    }
}
