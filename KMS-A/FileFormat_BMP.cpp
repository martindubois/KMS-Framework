
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/FileFormat_BMP.cpp

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

#define MAGIC 0x4D42 // 'MB'

namespace KMS
{
    namespace FileFormat
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        BMP::BMP(const File::Folder& aFolder, const char* aFile, bool aWrite)
            : mData(nullptr)
            , mFile(aFolder, aFile, aWrite)
            , mHeader(nullptr)
        {}

        const void* BMP::GetData() const { return mData; }
              void* BMP::GetData()       { return mData; }

        unsigned int BMP::GetDataSize_byte() const
        {
            assert(nullptr != mHeader);

            return mHeader->mSizeX_Low_px * mHeader->mSizeY_Low_px * BYTE_PER_PIXEL;
        }

        const void * BMP::GetHeader() const { return mHeader; }

        unsigned int BMP::GetSize_byte() const { return FULL_HEADER_SIZE_byte + GetDataSize_byte(); }
        unsigned int BMP::GetSizeX_px () const { assert(nullptr != mHeader); return mHeader->mSizeX_Low_px; }
        unsigned int BMP::GetSizeY_px () const { assert(nullptr != mHeader); return mHeader->mSizeY_Low_px; }

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

            KMS_EXCEPTION_ASSERT(MAGIC                 == mHeader->mMagic                  , RESULT_FILE_FORMAT_ERROR, "Invalid magic"           , mHeader->mMagic);
            KMS_EXCEPTION_ASSERT(FULL_HEADER_SIZE_byte == mHeader->mDataOffset_Low_byte    , RESULT_FILE_FORMAT_ERROR, "Invalid data offset"     , mHeader->mDataOffset_Low_byte);
            KMS_EXCEPTION_ASSERT(INFO_HEADER_SIZE_byte == mHeader->mInfoHeaderSize_Low_byte, RESULT_FILE_FORMAT_ERROR, "Invalid info header size", mHeader->mInfoHeaderSize_Low_byte);
            KMS_EXCEPTION_ASSERT(                    0 <  mHeader->mSizeX_Low_px           , RESULT_FILE_FORMAT_ERROR, "Invalid size X"          , mHeader->mSizeX_Low_px);
            KMS_EXCEPTION_ASSERT(                    0 <  mHeader->mSizeY_Low_px           , RESULT_FILE_FORMAT_ERROR, "Invalid size Y"          , mHeader->mSizeY_Low_px);
            KMS_EXCEPTION_ASSERT(BIT_PER_PIXEL         == mHeader->mBitPerPixel            , RESULT_FILE_FORMAT_ERROR, "Invalid bit per pixel"   , mHeader->mBitPerPixel);
            KMS_EXCEPTION_ASSERT(                    0 == mHeader->mCompression_Low        , RESULT_FILE_FORMAT_ERROR, "Invalid compression"     , mHeader->mCompression_Low);
        }

    }
}
