
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/FileFormat_BMP.cpp

// TEST COVERAGE  2023-08-24  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/FileFormat/BMP.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define BIT_PER_PIXEL  (24)
#define BYTE_PER_PIXEL ( 3)

#define INFO_HEADER_SIZE_byte (40)

#define FULL_HEADER_SIZE_byte (14 + INFO_HEADER_SIZE_byte)

#define MAGIC 'MB'

namespace KMS
{
    namespace FileFormat
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const uint32_t BMP::BLACK = 0x00000000;
        const uint32_t BMP::BLUE  = 0x000000ff;
        const uint32_t BMP::GREEN = 0x0000ff00;
        const uint32_t BMP::RED   = 0x00ff0000;
        const uint32_t BMP::WHITE = 0x00ffffff;

        uint32_t BMP::MakeColor(const uint8_t* aBlue)
        {
            assert(nullptr != aBlue);

            return *reinterpret_cast<const uint32_t*>(aBlue) & WHITE;
        }

        // NOT TESTED
        uint32_t BMP::MakeColor(uint8_t aRed, uint8_t aGreen, uint8_t aBlue)
        {
            uint32_t lResult = aRed;

            lResult <<= 8;
            lResult |= aGreen;
            lResult <<= 8;
            lResult |= aBlue;

            return lResult;
        }

        BMP::BMP(const File::Folder& aFolder, const char* aFile, bool aWrite)
            : mData(nullptr)
            , mFile(aFolder, aFile, aWrite)
            , mHeader(nullptr)
        {}

        BMP::~BMP() {}

        const void* BMP::GetData() const { return mData; }

        unsigned int BMP::GetDataSize_byte() const
        {
            assert(nullptr != mHeader);

            return mHeader->mSizeX_Low_px * mHeader->mSizeY_Low_px * BYTE_PER_PIXEL;
        }

        const void * BMP::GetHeader() const { return mHeader; }

        uint32_t BMP::GetPixel(uint16_t aX_px, uint16_t aY_px) const
        {
            auto lOffset_byte = ComputeOffset(aX_px, aY_px);

            return MakeColor(mData + lOffset_byte);
        }

        unsigned int BMP::GetSize_byte() const { return FULL_HEADER_SIZE_byte + GetDataSize_byte(); }
        unsigned int BMP::GetSizeX_px () const { assert(nullptr != mHeader); return mHeader->mSizeX_Low_px; }
        unsigned int BMP::GetSizeY_px () const { assert(nullptr != mHeader); return mHeader->mSizeY_Low_px; }

        void BMP::SetLine(uint16_t aX0_px, uint16_t aY0_px, uint16_t aX1_px, uint16_t aY1_px, uint32_t aValue, Operation aOp)
        {
            if ((aX0_px == aX1_px) || (aY0_px == aY1_px))
            {
                SetBox(aX0_px, aY0_px, aX1_px, aY1_px, aValue, aOp);
            }
            else
            {
                int lX0_px = aX0_px;
                int lY0_px = aY0_px;

                int lX1_px = aX1_px;
                int lY1_px = aY1_px;

                int lDX_px = lX1_px - lX0_px;
                int lDY_px = lY1_px - lY0_px;

                int lADX_px = (0 > lDX_px) ? -lDX_px : lDX_px;
                int lADY_px = (0 > lDY_px) ? -lDY_px : lDY_px;

                int lAD_px = (lADX_px < lADY_px) ? lADY_px : lADX_px;

                for (int i = 0; i <= lAD_px; i++)
                {
                    unsigned int x = lX0_px + i * lDX_px / lAD_px;
                    unsigned int y = lY0_px + i * lDY_px / lAD_px;

                    SetPixel(x, y, aValue, aOp);
                }
            }
        }

        void BMP::SetBox(uint16_t aX0_px, uint16_t aY0_px, uint16_t aX1_px, uint16_t aY1_px, uint32_t aValue, Operation aOp)
        {
            assert(aX0_px <= aX1_px);
            assert(aY0_px <= aY1_px);

            for (auto y = aY0_px; y <= aY1_px; y++)
            {
                for (auto x = aX0_px; x <= aX1_px; x++)
                {
                    SetPixel(x, y, aValue, aOp);
                }
            }
        }

        void BMP::SetPixel(uint16_t aX_px, uint16_t aY_px, uint32_t aValue, Operation aOp)
        {
            auto lOffset_byte = ComputeOffset(aX_px, aY_px);

            auto lData8 = mData + lOffset_byte;

            auto lData32 = reinterpret_cast<uint32_t*>(lData8);

            auto lValue8 = reinterpret_cast<uint8_t*>(&aValue);

            unsigned int i;

            switch (aOp)
            {
            case Operation::OP_AND : *lData32 &= (aValue | 0xff000000); break;
            case Operation::OP_COPY: *lData32 &= WHITE; *lData32 |= aValue; break;
            case Operation::OP_OR  : *lData32 |= aValue; break;
            case Operation::OP_XOR : *lData32 ^= aValue; break;

            case Operation::OP_ADD:
                for (i = 0; i < BYTE_PER_PIXEL; i++)
                {
                    lData8[i] += lValue8[i];
                }
                break;

            case Operation::OP_SUB:
                for (i = 0; i < BYTE_PER_PIXEL; i++)
                {
                    lData8[i] -= lValue8[i];
                }
                break;

            default: assert(false);
            }
        }

        void BMP::Close() { mFile.Close(); }

        void BMP::Create(uint16_t aSizeX_px, uint16_t aSizeY_px)
        {
            assert(nullptr == mHeader);

            unsigned int lSize_byte = FULL_HEADER_SIZE_byte + aSizeX_px * aSizeY_px * BYTE_PER_PIXEL;

            mHeader = reinterpret_cast<Header*>(mFile.Map(lSize_byte, lSize_byte));

            memset(mHeader, 0, lSize_byte);

            Header_Init(aSizeX_px, aSizeY_px);

            Data_Init();
        }

        void BMP::Open()
        {
            assert(nullptr == mHeader);

            mHeader = reinterpret_cast<Header*>(mFile.Map(sizeof(Header)));
            assert(nullptr != mHeader);

            Header_Validate();

            Data_Init();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int BMP::ComputeOffset(uint16_t aX_px, uint16_t aY_px) const
        {
            assert(nullptr != mHeader);

            assert(aX_px < mHeader->mSizeX_Low_px);

            unsigned int lResult_byte = aY_px;

            lResult_byte *= mHeader->mSizeX_Low_px;
            lResult_byte += aX_px;
            lResult_byte *= BYTE_PER_PIXEL;

            return lResult_byte;
        }

        void BMP::Data_Init()
        {
            assert(nullptr == mData);
            assert(nullptr != mHeader);

            mData = reinterpret_cast<uint8_t*>(mHeader + 1);
        }

        void BMP::Header_Init(uint16_t aSizeX_px, uint16_t aSizeY_px)
        {
            assert(0 < aSizeX_px);
            assert(0 < aSizeY_px);

            assert(nullptr != mHeader);

            unsigned int lDataSize_byte = aSizeX_px * aSizeY_px * BYTE_PER_PIXEL;

            unsigned int lSize_byte = FULL_HEADER_SIZE_byte + lDataSize_byte;

            mHeader->mMagic                   = MAGIC;
            mHeader->mSize_Low_byte           = lSize_byte & 0x0000ffff;
            mHeader->mSize_High               = lSize_byte >> 16;
            mHeader->mDataOffset_Low_byte     = FULL_HEADER_SIZE_byte;
            mHeader->mInfoHeaderSize_Low_byte = INFO_HEADER_SIZE_byte;
            mHeader->mSizeX_Low_px            = aSizeX_px;
            mHeader->mSizeY_Low_px            = aSizeY_px;
            mHeader->mPlaneCount              = 1;
            mHeader->mBitPerPixel             = BIT_PER_PIXEL;
        }

        void BMP::Header_Validate()
        {
            assert(nullptr != mHeader);

            KMS_EXCEPTION_ASSERT(MAGIC                 == mHeader->mMagic                  , FILE_FORMAT_ERROR, "Invalid magic"           , mHeader->mMagic);
            KMS_EXCEPTION_ASSERT(FULL_HEADER_SIZE_byte == mHeader->mDataOffset_Low_byte    , FILE_FORMAT_ERROR, "Invalid data offset"     , mHeader->mDataOffset_Low_byte);
            KMS_EXCEPTION_ASSERT(INFO_HEADER_SIZE_byte == mHeader->mInfoHeaderSize_Low_byte, FILE_FORMAT_ERROR, "Invalid info header size", mHeader->mInfoHeaderSize_Low_byte);
            KMS_EXCEPTION_ASSERT(                    0 <  mHeader->mSizeX_Low_px           , FILE_FORMAT_ERROR, "Invalid size X"          , mHeader->mSizeX_Low_px);
            KMS_EXCEPTION_ASSERT(                    0 <  mHeader->mSizeY_Low_px           , FILE_FORMAT_ERROR, "Invalid size Y"          , mHeader->mSizeY_Low_px);
            KMS_EXCEPTION_ASSERT(BIT_PER_PIXEL         == mHeader->mBitPerPixel            , FILE_FORMAT_ERROR, "Invalid bit per pixel"   , mHeader->mBitPerPixel);
            KMS_EXCEPTION_ASSERT(                    0 == mHeader->mCompression_Low        , FILE_FORMAT_ERROR, "Invalid compression"     , mHeader->mCompression_Low);
        }

    }
}
