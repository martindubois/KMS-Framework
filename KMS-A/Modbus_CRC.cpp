
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

        bool CRC::Verify(const uint8_t* aBuffer, unsigned int aSize_byte)
        {
            assert(CRC_SIZE_byte < aSize_byte);

            unsigned int lSize_byte = aSize_byte - CRC_SIZE_byte;

            CRC lCRC;

            lCRC.Compute(aBuffer, lSize_byte);

            uint16_t lReceived = aBuffer[lSize_byte + 1];

            lReceived <<= 8;
            lReceived |= aBuffer[lSize_byte];

            #ifdef _KMS_EMBEDDED_
                return (lCRC == lReceived);
            #else
                char lMsg[128];
                sprintf_s(lMsg, "Bad Modbus CRC (Expected = 0x%04x, Received = 0x%04x)", static_cast<unsigned int>(lCRC), lReceived);
                KMS_EXCEPTION_ASSERT(lCRC == lReceived, MODBUS_CRC_ERROR, lMsg, aSize_byte);
                return true;
            #endif
        }

        CRC::CRC() { Reset(); }

        CRC::operator uint16_t () const { return mValue; }

        void CRC::Compute(uint8_t aByte)
        {
            mValue ^= aByte;

            for (unsigned int j = 0; j < 8; j++)
            {
                auto lCarry = 1 == (mValue & 0x0001);

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
