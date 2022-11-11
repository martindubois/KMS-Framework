
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_FrameBuffer.cpp

// TEST COVERAGE 2022-11-10 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        FrameBuffer::FrameBuffer() { Clear(); }

        void FrameBuffer::AddDataByte(uint8_t aIn)
        {
            assert(sizeof(mHeader) <= mFrameSize_byte);

            mData[mFrameSize_byte - sizeof(mHeader)] = aIn;
            mFrameSize_byte++;
        }

        void FrameBuffer::AddDataBytes(const void* aIn, uint8_t aInSize_byte)
        {
            assert(NULL != aIn);

            const uint8_t* lIn = reinterpret_cast<const uint8_t*>(aIn);

            for (unsigned int i = 0; i < aInSize_byte; i++)
            {
                AddDataByte(lIn[i]);
            }
        }

        uint8_t FrameBuffer::GetData(void* aOut, uint8_t aOutSize_byte) const
        {
            assert(NULL != aOut);

            if (mHeader.mFields.mDataSize_byte > aOutSize_byte)
            {
                // NOT TESTED
                return KMS_WOP_RESULT_INVALID_DATA_SIZE;
            }

            memcpy(aOut, mData, mHeader.mFields.mDataSize_byte);

            return KMS_WOP_RESULT_OK;
        }

        uint8_t FrameBuffer::GetDataType() const { return mHeader.mFields.mDataType; }
        uint8_t FrameBuffer::GetInstance() const { return mHeader.mFields.mInstance; }

        void FrameBuffer::Prepare(uint8_t aInstance, uint8_t aDataType)
        {
            mHeader.mFields.mControl       = KMS_WOP_CONTROL_VALUE;
            mHeader.mFields.mDataSize_byte = 0;
            mHeader.mFields.mDataType      = aDataType;
            mHeader.mFields.mInstance      = aInstance;
            mHeader.mFields.mReserved0     = 0;

            mFrameSize_byte = sizeof(mHeader);
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        uint8_t FrameBuffer::AddReceivedByte(uint8_t aIn)
        {
            CRC_Compute(aIn);

            switch (mFrameSize_byte)
            {
            case 0:
                mHeader.mBytes[mFrameSize_byte] = aIn;
                if (KMS_WOP_CONTROL_VALUE != mHeader.mFields.mControl)
                {
                    return KMS_WOP_RESULT_INVALID_CONTROL_VALUE;
                }
                break;

            case 1:
            case 2:
                mHeader.mBytes[mFrameSize_byte] = aIn;
                break;

            case 3:
                mHeader.mBytes[mFrameSize_byte] = aIn;
                mExpectedFrameSize_byte = sizeof(mHeader) + mHeader.mFields.mDataSize_byte + sizeof(uint16_t);
                break;

            default:
                if (sizeof(mHeader) + sizeof(mData) <= mFrameSize_byte)
                {
                    // NOT TESTED
                    return KMS_WOP_RESULT_INVALID_FRAME_SIZE;
                }

                mData[mFrameSize_byte - sizeof(mHeader)] = aIn;
            }

            mFrameSize_byte++;

            if (mExpectedFrameSize_byte <= mFrameSize_byte)
            {
                return CRC_Verify();
            }

            return KMS_WOP_RESULT_PARTIAL;
        }

        void FrameBuffer::Clear()
        {
            mExpectedFrameSize_byte = 0xffff;
            mFrameSize_byte = 0;

            CRC_Reset();
        }

        void FrameBuffer::Seal()
        {
            mHeader.mFields.mDataSize_byte = mFrameSize_byte - sizeof(mHeader);

            CRC_Reset();

            unsigned int i;

            for (i = 0; i < sizeof(mHeader); i++)
            {
                CRC_Compute(mHeader.mBytes[i]);
            }

            for (i = 0; i < mHeader.mFields.mDataSize_byte; i++)
            {
                CRC_Compute(mData[i]);
            }

            mData[mHeader.mFields.mDataSize_byte    ] = mCRC & 0xff;
            mData[mHeader.mFields.mDataSize_byte + 1] = mCRC >> 8;

            mFrameSize_byte += sizeof(uint16_t);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void FrameBuffer::CRC_Compute(uint8_t aIn)
        {
            mCRC ^= aIn;

            for (unsigned int j = 0; j < 8; j++)
            {
                bool lCarry = 1 == (mCRC & 0x0001);

                mCRC >>= 1;

                if (lCarry)
                {
                    mCRC ^= 0xa001;
                }
            }
        }

        void FrameBuffer::CRC_Reset() { mCRC = 0xffff; }

        uint8_t FrameBuffer::CRC_Verify()
        {
            if (0 != mCRC)
            {
                return KMS_WOP_RESULT_INVALID_CRC;
            }

            return KMS_WOP_RESULT_OK;
        }

    }
}
