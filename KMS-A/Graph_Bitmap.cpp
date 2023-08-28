
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Graph_Bitmap.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Graph/Bitmap.h>

// Data type
// //////////////////////////////////////////////////////////////////////////

class Point_Int
{

public:

    Point_Int(int aX_px, int aY_px);

    Point_Int(KMS::Graph::Point aP);

    operator KMS::Graph::Point() const;

    Point_Int operator - (Point_Int aB) const;

    Point_Int Abs() const;

    int mX_px;
    int mY_px;

};

// Constants
// //////////////////////////////////////////////////////////////////////////

#define PIXEL_SIZE_byte (3)

#define FLAG_DELETE_DATA (0x00000001)

namespace KMS
{
    namespace Graph
    {

        Bitmap::Bitmap() : mData(nullptr), mFlags(0), mSizeX_px(0), mSizeY_px(0) {}

        Bitmap::~Bitmap()
        {
            if (0 != (mFlags & FLAG_DELETE_DATA))
            {
                assert(nullptr != mData);

                delete[] mData;
            }
        }

        void Bitmap::Init(unsigned int aSizeX_px, unsigned int aSizeY_px)
        {
            assert(0 < aSizeX_px);
            assert(0 < aSizeY_px);

            assert(nullptr == mData);
            assert(0 == mSizeX_px);
            assert(0 == mSizeY_px);

            unsigned int lSize_byte = aSizeX_px * aSizeY_px * PIXEL_SIZE_byte;

            mData = new uint8_t[lSize_byte];

            mFlags |= FLAG_DELETE_DATA;

            mSizeX_px = aSizeX_px;
            mSizeY_px = aSizeY_px;

            memset(mData, 0, lSize_byte);
        }

        void Bitmap::Init(FileFormat::BMP* aBMP)
        {
            assert(nullptr != aBMP);

            assert(nullptr == mData);
            assert(0 == mSizeX_px);
            assert(0 == mSizeY_px);

            mData = reinterpret_cast<uint8_t*>(aBMP->GetData());

            mSizeX_px = aBMP->GetSizeX_px();
            mSizeY_px = aBMP->GetSizeY_px();
        }

        const void* Bitmap::GetData() const { return mData; }

        Color Bitmap::GetPixel(Point aP) const
        {
            auto lOffset_byte = ComputeOffset(aP);

            return Color(mData + lOffset_byte);
        }

        unsigned int Bitmap::GetSizeX_px() const { return mSizeX_px; }
        unsigned int Bitmap::GetSizeY_px() const { return mSizeY_px; }

        void Bitmap::SetBox(Point aP0, Point aP1, Color aColor, Operation aOp)
        {
            assert(aP0.mX_px <= aP1.mX_px);
            assert(aP0.mY_px <= aP1.mY_px);

            for (auto y = aP0.mY_px; y <= aP1.mY_px; y++)
            {
                for (auto x = aP0.mX_px; x <= aP1.mX_px; x++)
                {
                    SetPixel(Point(x, y), aColor, aOp);
                }
            }
        }

        void Bitmap::SetLine(Point aP0, Point aP1, Color aColor, Operation aOp)
        {
            if ((aP0.mX_px == aP1.mX_px) || (aP0.mY_px == aP1.mY_px))
            {
                SetBox(aP0, aP1, aColor, aOp);
            }
            else
            {
                Point_Int lP0(aP0);
                Point_Int lP1(aP1);

                Point_Int lD = lP1 - lP0;

                Point_Int lAD = lD.Abs();

                int lAD_px = (lAD.mX_px < lAD.mY_px) ? lAD.mY_px : lAD.mX_px;

                for (int i = 0; i <= lAD_px; i++)
                {
                    Point lP(lP0.mX_px + i * lD.mX_px / lAD_px, lP0.mY_px + i * lD.mY_px / lAD_px);

                    SetPixel(lP, aColor, aOp);
                }
            }
        }

        void Bitmap::SetPixel(Point aP, Color aColor, Operation aOp)
        {
            auto lOffset_byte = ComputeOffset(aP);

            auto lColor8 = reinterpret_cast<uint8_t*>(&aColor);

            uint32_t lColor32 = aColor;

            auto lData8 = mData + lOffset_byte;

            auto lData32 = reinterpret_cast<uint32_t*>(lData8);

            unsigned int i;

            switch (aOp)
            {
            case Operation::OP_AND : *lData32 &= (lColor32 | 0xff000000); break;
            case Operation::OP_COPY: *lData32 &= 0xffffff; *lData32 |= lColor32; break;
            case Operation::OP_OR  : *lData32 |= lColor32; break;
            case Operation::OP_XOR : *lData32 ^= lColor32; break;

            case Operation::OP_ADD:
                for (i = 0; i < PIXEL_SIZE_byte; i++)
                {
                    lData8[i] += lColor8[i];
                }
                break;

            case Operation::OP_SUB:
                for (i = 0; i < PIXEL_SIZE_byte; i++)
                {
                    lData8[i] -= lColor8[i];
                }
                break;

            default: assert(false);
            }
        }

        void Bitmap::Scroll_Left(unsigned int aScroll_px, Color aValue, Operation aOp)
        {
            assert(0 < aScroll_px);

            assert(nullptr != mData);

            assert(0 < mSizeX_px);
            assert(0 < mSizeY_px);

            auto lScroll_byte = aScroll_px * PIXEL_SIZE_byte;

            auto lDst = mData;
            auto lSrc = lDst + lScroll_byte;

            unsigned int lLine_byte = mSizeX_px * PIXEL_SIZE_byte;
            unsigned int lMove_byte = lLine_byte - lScroll_byte;

            for (uint16_t y = 0; y < mSizeY_px; y++)
            {
                memmove(lDst, lSrc, lMove_byte);

                lDst += lLine_byte;
                lSrc += lLine_byte;
            }

            Point_Int lLast(mSizeX_px - 1, mSizeY_px - 1);

            SetBox(Point(lLast.mX_px - aScroll_px, 0), lLast, aValue, aOp);
        }

        void Bitmap::Scroll_Right(unsigned int aScroll_px, Color aValue, Operation aOp)
        {
            assert(0 < aScroll_px);

            assert(nullptr != mData);

            assert(0 < mSizeX_px);
            assert(0 < mSizeY_px);

            auto lScroll_byte = aScroll_px * PIXEL_SIZE_byte;

            auto lSrc = mData;
            auto lDst = mData + lScroll_byte;

            unsigned int lLine_byte = mSizeX_px * PIXEL_SIZE_byte;
            unsigned int lMove_byte = lLine_byte - lScroll_byte;

            for (uint16_t y = 0; y < mSizeY_px; y++)
            {
                memmove(lDst, lSrc, lMove_byte);

                lDst += lLine_byte;
                lSrc += lLine_byte;
            }

            SetBox(Point::ORIGIN, Point(aScroll_px, mSizeY_px - 1), aValue, aOp);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int Bitmap::ComputeOffset(Point aP) const
        {
            assert(aP.mX_px < mSizeX_px);

            unsigned int lResult_byte = aP.mY_px;

            lResult_byte *= mSizeX_px;
            lResult_byte += aP.mX_px;
            lResult_byte *= PIXEL_SIZE_byte;

            return lResult_byte;
        }

    }
}

using namespace KMS;

// Internal class
// //////////////////////////////////////////////////////////////////////////

Point_Int::Point_Int(int aX_px, int aY_px) : mX_px(aX_px), mY_px(aY_px)
{}

Point_Int::Point_Int(Graph::Point aP) : mX_px(aP.mX_px), mY_px(aP.mY_px)
{}

Point_Int::operator Graph::Point() const
{
    return Graph::Point(mX_px, mY_px);
}

Point_Int Point_Int::operator - (Point_Int aB) const
{
    return Point_Int(mX_px - aB.mX_px, mY_px - aB.mY_px);
}

Point_Int Point_Int::Abs() const
{
    return Point_Int(0 > mX_px ? -mX_px : mX_px, 0 > mY_px ? -mY_px : mY_px);
}
