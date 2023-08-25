
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Graph/Bitmap.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/FileFormat/BMP.h>
#include <KMS/Graph/Color.h>
#include <KMS/Graph/Point.h>

namespace KMS
{
    namespace Graph
    {

        class Bitmap
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

            Bitmap();

            ~Bitmap();

            void Init(unsigned int aSizeX_px, unsigned int aSizeY_px);

            void Init(unsigned int aSizeX_px, unsigned int aSizeY_px, void* aData);

            void Init(FileFormat::BMP* aBMP);

            Color GetPixel(Point aP) const;

            unsigned int GetSizeX_px() const;
            unsigned int GetSizeY_px() const;

            // aP0  Bottom left corner
            // aP1  Upper right corner
            void SetBox(Point aP0, Point aP1, Color aColor, Operation aOp = Operation::OP_COPY);

            void SetLine(Point aP0, Point aP1, Color aColor, Operation aOp = Operation::OP_COPY);

            void SetPixel(Point aP, Color aColor, Operation aOp = Operation::OP_COPY );

            void Scroll_Left (unsigned int aScroll_px, Color aColor = Color::BLACK, Operation aOp = Operation::OP_COPY);
            void Scroll_Right(unsigned int aScroll_px, Color aColor = Color::BLACK, Operation aOp = Operation::OP_COPY);

        private:

            unsigned int ComputeOffset(Point aP) const;

            uint8_t* mData;

            unsigned int mFlags;

            unsigned int mSizeX_px;
            unsigned int mSizeY_px;

        };

    }
}

