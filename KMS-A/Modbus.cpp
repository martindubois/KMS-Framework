
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/Modbus.h>

namespace KMS
{
    namespace Modbus
    {

        // Constants
        // //////////////////////////////////////////////////////////////////

        const uint8_t BROADCAST = 0;

        const uint16_t OFF = 0x0000;
        const uint16_t ON = 0xff00;

        // Functions
        // //////////////////////////////////////////////////////////////////

        bool ReadBit(const uint8_t* aData, unsigned int aOffset, unsigned int aIndex)
        {
            assert(NULL != aData);

            uint8_t lByte = aData[aOffset + aIndex / 8];

            return 0 != (lByte & (1 << (aIndex % 8)));
        }

        void WriteBit(uint8_t* aData, unsigned int aOffset, unsigned aIndex, bool aValue)
        {
            uint8_t* lByte = aData + aOffset + aIndex / 8;

            uint8_t lBit = 1 << (aIndex % 8);

            if (aValue)
            {
                *lByte |= lBit;
            }
            else
            {
                *lByte &= ~lBit;
            }
        }

        uint8_t ReadUInt8(const uint8_t* aData, unsigned int aOffset)
        {
            assert(NULL != aData);

            return aData[aOffset];
        }

        uint16_t ReadUInt16(const uint8_t* aData, unsigned int aOffset)
        {
            assert(NULL != aData);

            uint16_t lResult;

            lResult = aData[aOffset] << 8;
            lResult |= aData[aOffset + 1];

            return lResult;
        }

        void WriteUInt8(uint8_t* aData, unsigned int aOffset, uint8_t aValue)
        {
            assert(NULL != aData);

            aData[aOffset] = aValue;
        }

        void WriteUInt16(uint8_t* aData, unsigned int aOffset, uint16_t aValue)
        {
            assert(NULL != aData);

            aData[aOffset] = aValue >> 8;
            aData[aOffset + 1] = aValue & 0xff;
        }

    }
}
