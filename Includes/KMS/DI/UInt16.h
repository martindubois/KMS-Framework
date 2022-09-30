
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/UInt16.h
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

        class UInt16 : public Value
        {

        public:

            static DI::Object* Create();

            UInt16(uint16_t aIn = 0);

            const UInt16& operator = (uint16_t aIn);

            operator uint16_t () const;

            bool operator == (uint16_t aIn);

            void Clear();

            uint16_t Get() const;

            void SetRadix(Radix aRadix);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~UInt16();

        private:

            uint16_t mInternal;

            Radix mRadix;

        };

    }
}
