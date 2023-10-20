
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/FrameBuffer.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/WOP/WOP.h>

namespace KMS
{
    namespace WOP
    {

        class FrameBuffer final
        {

        public:

            FrameBuffer();

            void AddDataByte(uint8_t aIn);

            void AddDataBytes(const void* aIn, uint8_t aInSize_byte);

            uint8_t GetData(void* aOut, uint8_t aOutSize_byte) const;

            uint8_t GetDataSize_byte() const;

            uint8_t GetDataType() const;

            unsigned int GetFrameSize_byte() const;

            uint8_t GetInstance() const;

            const void* GetRawFrame() const;
            const void* GetRawData () const;

            void Prepare(uint8_t aInstance, uint8_t aDataType);

        // Internal

            uint8_t AddReceivedByte(uint8_t aIn);

            uint8_t AddReceivedBytes(const void* aIn, unsigned int aInSize_byte);

            void Clear();
            void Seal();

        private:

            void    CRC_Compute(uint8_t aIn);
            void    CRC_Reset  ();
            uint8_t CRC_Verify ();

            KMS_WOP_Header mHeader;
            uint8_t        mData[255 + 2];

            uint16_t     mCRC;
            unsigned int mExpectedFrameSize_byte;
            unsigned int mFrameSize_byte;

        };

    }
}