
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/FileFormat/BMP.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Binary.h>
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace FileFormat
    {

        class BMP
        {

        public:

            enum class Operation
            {
                OP_ADD = 0,
                OP_AND,
                OP_COPY,
                OP_OR,
                OP_SUB,
                OP_XOR,

                QTY
            };

            static const uint32_t BLACK;
            static const uint32_t BLUE;
            static const uint32_t GREEN;
            static const uint32_t RED;
            static const uint32_t WHITE;

            static uint32_t MakeColor(const uint8_t* aBlue);
            static uint32_t MakeColor(uint8_t aRed, uint8_t aGreen, uint8_t aBlue);

            BMP(const File::Folder& aFolder, const char* aFile, bool aWrite = false);

            ~BMP();

            const void* GetData() const;

            unsigned int GetDataSize_byte() const;

            const void* GetHeader() const;

            uint32_t GetPixel(uint16_t aX_px, uint16_t aY_px) const;

            unsigned int GetSize_byte() const;

            unsigned int GetSizeX_px() const;
            unsigned int GetSizeY_px() const;

            void SetBox(uint16_t aX0_px, uint16_t aY0_px, uint16_t aX1_px, uint16_t aX2_px, uint32_t aValue, Operation aOp = Operation::OP_COPY);

            void SetLine(uint16_t aX0_px, uint16_t aY0_px, uint16_t aX1_px, uint16_t aX2_px, uint32_t aValue, Operation aOp = Operation::OP_COPY);

            void SetPixel(uint16_t aX_px, uint16_t aY_px, uint32_t aValue, Operation aOp = Operation::OP_COPY );

            void Close();

            void Create(uint16_t aSizeX_px, uint16_t aSizeY_px);

            void Open();

        private:

            // See https://en.wikipedia.org/wiki/BMP_file_format
            typedef struct
            {
                uint16_t mMagic;                   // 00 00 Must be 'MB'
                uint16_t mSize_Low_byte;           // 02 02
                uint16_t mSize_High;
                uint16_t mReserved0[2];            // 06 06 Ignored and set to 0
                uint16_t mDataOffset_Low_byte;     // 10 0a Must be 14 + 40 = 54
                uint16_t mDataOffset_High;         //       Must be 0 (not validated)

                uint16_t mInfoHeaderSize_Low_byte; // 14 0e Must be 40
                uint16_t mInfoHeaderSize_High;     //       Must be 0 (not validated)
                int16_t  mSizeX_Low_px;            // 18 12
                uint16_t mSizeX_High;              //       Must be 0 (not validated)
                int16_t  mSizeY_Low_px;            // 22 16
                uint16_t mSizeY_High;              //       Must be 0 (not validated)
                uint16_t mPlaneCount;              // 26 1a Must be 1 (not validated)
                uint16_t mBitPerPixel;             // 28 1c Must be 24
                uint16_t mCompression_Low;         // 30 1e Must be 0
                uint16_t mCompression_High;        //       Must be 0 (not validated)
                uint16_t mDataSize_Low_byte;       // 34 22 Ignored and set to 0
                uint16_t mDataSize_High;
                int16_t  mResolutionX_Low_px_m;    // 38 26 Ignored
                uint16_t mResulutionX_High;
                int16_t  mResolutionY_Low_px_m;    // 42 2a Ignored
                uint16_t mResulutionY_High;
                uint16_t mPaletteLength_Low;       // 46 2e Ignored and set to 0
                uint16_t mPaletteLength_High;
                uint16_t mImportantColorCount_Low; // 50 32 Ignored and set to 0
                uint16_t mImportantColorCount_High;
            }
            Header;

            unsigned int ComputeOffset(uint16_t aX_px, uint16_t aY_px) const;

            void Data_Init();

            void Header_Init(uint16_t aSizeX_px, uint16_t aSizeY_px);
            void Header_Validate();

            uint8_t    * mData;
            File::Binary mFile;
            Header     * mHeader;

        };

    }
}
