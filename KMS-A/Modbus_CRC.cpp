
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_CRC.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/Modbus.h>

#include <KMS/Modbus/CRC.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define CRC_SIZE_byte (2)

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void CRC::Add(uint8_t* aBuffer, unsigned int aSize_byte)
        {
            assert(CRC_SIZE_byte < aSize_byte);

            CRC lCRC;

            unsigned int lSize_byte = aSize_byte - CRC_SIZE_byte;

            lCRC.Compute(aBuffer, lSize_byte);

            uint16_t lValue = lCRC;

            WriteUInt8(aBuffer, lSize_byte    , lValue & 0xff);
            WriteUInt8(aBuffer, lSize_byte + 1, lValue >> 8);
        }

        void Verify(const uint8_t* aBuffer, unsigned int aSize_byte)
        {
            assert(CRC_SIZE_byte < aSize_byte);

            unsigned int lSize_byte = aSize_byte - CRC_SIZE_byte;

            CRC lCRC;

            lCRC.Compute(aBuffer, lSize_byte);

            if (lCRC != KMS::Modbus::ReadUInt16(aBuffer, lSize_byte))
            {
                KMS_EXCEPTION(MODBUS_CRC, "Bad Modbus CRC");
            }
        }


        CRC::CRC() { Reset(); }

        CRC::operator uint16_t () const { return mValue; }

        void CRC::Compute(uint8_t aByte)
        {
            mValue ^= aByte;

            for (unsigned int j = 0; j < 8; j++)
            {
                bool lCarry = 1 == (mValue & 0x0001);

                mValue >>= 1;

                if (lCarry)
                {
                    mValue ^= 0xa001;
                }
            }

        }

        void CRC::Compute(const uint8_t* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);

            for (unsigned int i = 0; i < aInSize_byte; i++)
            {
                Compute(aIn[i]);
            }
        }

        void CRC::Reset() { mValue = 0xffff; }

    }
}
