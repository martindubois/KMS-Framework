
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/CRC.h

#pragma once

namespace KMS
{
    namespace Modbus
    {

        class CRC final
        {

        public:

            static void Add(uint8_t* aBuffer, unsigned int aSize_byte);

            static bool Verify(const uint8_t* aBuffer, unsigned int aSize_byte);

            CRC();

            operator uint16_t () const;

            void Compute(uint8_t aByte);

            void Compute(const uint8_t* aIn, unsigned int aInSize_byte);

            void Reset();

        private:

            uint16_t mValue;

        };

    }
}
