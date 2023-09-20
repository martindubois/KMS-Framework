
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Graph_Bitmap.cpp

// TEST COVERAGE  2023-09-04  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Saturated.h>

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
#define FLAG_READ_ONLY   (0x00000002)

#define FONT_FIRST (32)

#define FONT_SIZE_X_px ( 8)
#define FONT_SIZE_Y_px (13)

static const uint8_t FONT[][FONT_SIZE_Y_px] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Space (32)
    { 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }, // !
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36 }, // "
    { 0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00 }, // #
    { 0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18 }, // $
    { 0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70 }, // %
    { 0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38 }, // &
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e }, // '
    { 0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c }, // (
    { 0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30 }, // )
    { 0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00 }, // *
    { 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00 }, // +
    { 0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // ,
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00 }, // -
    { 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // .
    { 0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03 }, // /
    { 0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c }, // 0
    { 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18 }, // 1
    { 0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e }, // 2
    { 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e }, // 3
    { 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c }, // 4
    { 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff }, // 5
    { 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e }, // 6
    { 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff }, // 7
    { 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e }, // 8
    { 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e }, // 9
    { 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00 }, // :
    { 0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00 }, // ;
    { 0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06 }, // <
    { 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 }, // =
    { 0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60 }, // >
    { 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e }, // ?
    { 0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00 }, // @
    { 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18 }, // A
    { 0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe }, // B
    { 0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e }, // C
    { 0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc }, // D
    { 0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff }, // E
    { 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff }, // F
    { 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e }, // G
    { 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 }, // H
    { 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e }, // I
    { 0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06 }, // J
    { 0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3 }, // K
    { 0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 }, // L
    { 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3 }, // M
    { 0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3 }, // N
    { 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e }, // O
    { 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe }, // P
    { 0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c }, // Q
    { 0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe }, // R
    { 0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e }, // S
    { 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff }, // T
    { 0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 }, // U
    { 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 }, // V
    { 0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 }, // W
    { 0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3 }, // X
    { 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3 }, // Y
    { 0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff }, // Z
    { 0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c }, // [
    { 0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60 }, // Back slash (92)
    { 0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c }, // ]
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18 }, // ^
    { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // _
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70 }, // `
    { 0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 }, // a
    { 0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0 }, // b
    { 0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 }, // c
    { 0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03 }, // d
    { 0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 }, // e
    { 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e }, // f
    { 0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00 }, // g
    { 0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0 }, // h
    { 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00 }, // i
    { 0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00 }, // j
    { 0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0 }, // k
    { 0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78 }, // l
    { 0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00 }, // m
    { 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00 }, // n
    { 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00 }, // o
    { 0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00 }, // p
    { 0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00 }, // q
    { 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00 }, // r
    { 0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00 }, // s
    { 0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00 }, // t
    { 0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00 }, // u
    { 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00 }, // v
    { 0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00 }, // w
    { 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00 }, // x
    { 0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00 }, // y
    { 0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00 }, // z
    { 0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f }, // {
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }, // |
    { 0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0 }, // }
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00 }, // ~
    { 0x00, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x00 }, // (127)
};

namespace KMS
{
    namespace Graph
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Bitmap::Bitmap()
            : mData(nullptr)
            , mFlags(0)
            , mSizeX_px(0)
            , mSizeY_px(0)
        {}

        Bitmap::~Bitmap()
        {
            if (0 != (mFlags & FLAG_DELETE_DATA))
            {
                assert(nullptr != mData);
                assert(0 == (mFlags & FLAG_READ_ONLY));

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

        void Bitmap::Init(unsigned int aSizeX_px, unsigned int aSizeY_px, const void* aData)
        {
            assert(0 < aSizeX_px);
            assert(0 < aSizeY_px);
            assert(nullptr != aData);

            assert(nullptr == mData);
            assert(0 == mSizeX_px);
            assert(0 == mSizeY_px);

            mData = reinterpret_cast<uint8_t*>(const_cast<void*>(aData));

            mFlags |= FLAG_READ_ONLY;

            mSizeX_px = aSizeX_px;
            mSizeY_px = aSizeY_px;
        }

        void Bitmap::Init(unsigned int aSizeX_px, unsigned int aSizeY_px, void* aData)
        {
            assert(0 < aSizeX_px);
            assert(0 < aSizeY_px);
            assert(nullptr != aData);

            assert(nullptr == mData);
            assert(0 == mSizeX_px);
            assert(0 == mSizeY_px);

            mData = reinterpret_cast<uint8_t*>(aData);

            mSizeX_px = aSizeX_px;
            mSizeY_px = aSizeY_px;
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

        Point Bitmap::GetUpperRight() const { return Point(mSizeX_px - 1, mSizeY_px - 1); }

        void Bitmap::SetBox(Point aP0, Point aP1, Color aColor, Operation aOp)
        {
            assert(aP0.mX_px <= aP1.mX_px);
            assert(aP0.mY_px <= aP1.mY_px);

            assert(nullptr != mData);
            assert(0 < mSizeX_px);
            assert(0 < mSizeY_px);

            if (mSizeX_px <= aP0.mX_px) { return; }
            if (mSizeY_px <= aP0.mY_px) { return; }

            auto lP1 = aP1;

            if (mSizeX_px <= aP1.mX_px) { lP1.mX_px = mSizeX_px - 1; }
            if (mSizeY_px <= aP1.mY_px) { lP1.mY_px = mSizeY_px - 1; }

            SetBoxContext lContext;

            lContext.mBoxSizeX_byte = (aP1.mX_px - aP0.mX_px + 1) * PIXEL_SIZE_byte;
            lContext.mBoxSizeY_px   = (aP1.mY_px - aP0.mY_px + 1);
            lContext.mColor         = reinterpret_cast<uint8_t*>(&aColor);
            lContext.mData          = mData + ComputeOffset(aP0);
            lContext.mSizeX_byte    = mSizeX_px * PIXEL_SIZE_byte;

            switch (aOp)
            {
            case Operation::OP_ADD : SetBox_ADD (&lContext); break;
            case Operation::OP_AND : SetBox_AND (&lContext); break;
            case Operation::OP_COPY: SetBox_COPY(&lContext); break;
            case Operation::OP_DIV : SetBox_DIV (&lContext); break;
            case Operation::OP_MULT: SetBox_MULT(&lContext); break;
            case Operation::OP_OR  : SetBox_OR  (&lContext); break;
            case Operation::OP_SUB : SetBox_SUB (&lContext); break;
            case Operation::OP_XOR : SetBox_XOR (&lContext); break;

            default: assert(false);
            }
        }

        void Bitmap::SetChar(char aC, Point aPoint, Color aColor, Operation aOp)
        {
            const uint8_t* lFont;
            
            if (FONT_FIRST > aC)
            {
                lFont = FONT[0];
            }
            else
            {
                lFont = FONT[aC - FONT_FIRST];
            };

            for (unsigned int y = 0; y < FONT_SIZE_Y_px; y++)
            {
                for (unsigned int x = 0; x < FONT_SIZE_X_px; x++)
                {
                    if (0 != (*lFont & (0x80 >> x)))
                    {
                        SetPixel(Point(aPoint.mX_px + x, aPoint.mY_px + y), aColor, aOp);
                    }
                }

                lFont++;
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
            KMS_EXCEPTION_ASSERT(0 == (mFlags & FLAG_READ_ONLY), RESULT_DENIED, "The bitmap is read only", "");

            if (mSizeX_px <= aP.mX_px) { return; }
            if (mSizeY_px <= aP.mY_px) { return; }

            auto lOffset_byte = ComputeOffset(aP);

            auto lColor8 = reinterpret_cast<uint8_t*>(&aColor);

            uint32_t lColor32 = aColor;

            auto lData8 = mData + lOffset_byte;

            auto lData32 = reinterpret_cast<uint32_t*>(lData8);

            unsigned int i;

            switch (aOp)
            {
            case Operation::OP_AND : *lData32 &= (lColor32 | 0xff000000); break;
            case Operation::OP_COPY: *lData32 &= 0xff000000; *lData32 |= lColor32; break;
            case Operation::OP_OR  : *lData32 |= lColor32; break;
            case Operation::OP_XOR : *lData32 ^= lColor32; break;

            case Operation::OP_ADD:
                for (i = 0; i < PIXEL_SIZE_byte; i++)
                {
                    lData8[i] = Saturated::Add(lData8[i], lColor8[i]);
                }
                break;

            case Operation::OP_DIV:
                for (i = 0; i < PIXEL_SIZE_byte; i++)
                {
                    lData8[i] = Saturated::Div(lData8[i], lColor8[i]);
                }
                break;

            case Operation::OP_MULT:
                for (i = 0; i < PIXEL_SIZE_byte; i++)
                {
                    lData8[i] = Saturated::Mul(lData8[i], lColor8[i]);
                }
                break;

            case Operation::OP_SUB:
                for (i = 0; i < PIXEL_SIZE_byte; i++)
                {
                    lData8[i] = Saturated::Sub(lData8[i], lColor8[i]);
                }
                break;

            default: assert(false);
            }
        }

        void Bitmap::SetString(const char* aStr, Point aPoint, Color aColor, Operation aOp)
        {
            assert(nullptr != aStr);

            Point lPoint = aPoint;
            const char* lStr = aStr;

            while ('\0' != *lStr)
            {
                switch (*lStr)
                {
                case '\n': lPoint.mX_px = aPoint.mX_px; lPoint.mY_px += FONT_SIZE_Y_px; break;
                case '\r': lPoint.mX_px = aPoint.mX_px; break;

                default: SetChar(*lStr, lPoint, aColor, aOp);
                }

                lStr++;
                lPoint.mX_px += FONT_SIZE_X_px;
            }
        }

        void Bitmap::Scroll_Left(unsigned int aScroll_px, Color aValue, Operation aOp)
        {
            assert(0 < aScroll_px);

            assert(nullptr != mData);

            assert(0 < mSizeX_px);
            assert(0 < mSizeY_px);

            KMS_EXCEPTION_ASSERT(0 == (mFlags & FLAG_READ_ONLY), RESULT_DENIED, "The bitmap is read only", "");

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

            SetBox(Point(lLast.mX_px - aScroll_px + 1, 0), lLast, aValue, aOp);
        }

        void Bitmap::Scroll_Right(unsigned int aScroll_px, Color aValue, Operation aOp)
        {
            assert(0 < aScroll_px);

            assert(nullptr != mData);

            assert(0 < mSizeX_px);
            assert(0 < mSizeY_px);

            KMS_EXCEPTION_ASSERT(0 == (mFlags & FLAG_READ_ONLY), RESULT_DENIED, "The bitmap is read only", "");
            
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

        void Bitmap::Scroll_Left(Point aP0, Point aP1, unsigned int aScroll_px, Color aValue, Operation aOp)
        {
            assert(aP0.mX_px <= aP1.mX_px);
            assert(aP0.mY_px <= aP1.mY_px);
            assert(0 < aScroll_px);

            assert(nullptr != mData);

            assert(0 < mSizeX_px);

            KMS_EXCEPTION_ASSERT(0 == (mFlags & FLAG_READ_ONLY), RESULT_DENIED, "The bitmap is read only", "");

            auto lScroll_byte = aScroll_px * PIXEL_SIZE_byte;

            auto lDst = mData + ComputeOffset(aP0);
            auto lSrc = lDst + lScroll_byte;

            unsigned int lLine_byte = mSizeX_px * PIXEL_SIZE_byte;
            unsigned int lMove_byte = (aP1.mX_px - aP0.mX_px + 1) * PIXEL_SIZE_byte - lScroll_byte;

            for (uint16_t y = aP0.mY_px; y <= aP1.mY_px; y++)
            {
                memmove(lDst, lSrc, lMove_byte);

                lDst += lLine_byte;
                lSrc += lLine_byte;
            }

            SetBox(Point(aP1.mX_px - aScroll_px, 0), aP1, aValue, aOp);
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

        void Bitmap::SetBox_ADD(SetBoxContext* aContext)
        {
            auto lColor = aContext->mColor;
            auto lData = aContext->mData;

            for (unsigned int y = 0; y < aContext->mBoxSizeY_px; y++)
            {
                for (unsigned int x = 0; x < aContext->mBoxSizeX_byte; x++)
                {
                    lData[x] = Saturated::Add(lData[x], lColor[x % PIXEL_SIZE_byte]);
                }

                lData += aContext->mSizeX_byte;
            }
        }

        void Bitmap::SetBox_AND(SetBoxContext* aContext)
        {
            auto lColor = aContext->mColor;
            auto lData = aContext->mData;

            for (unsigned int y = 0; y < aContext->mBoxSizeY_px; y++)
            {
                for (unsigned int x = 0; x < aContext->mBoxSizeX_byte; x++)
                {
                    lData[x] &= lColor[x % PIXEL_SIZE_byte];
                }

                lData += aContext->mSizeX_byte;
            }
        }

        void Bitmap::SetBox_COPY(SetBoxContext* aContext)
        {
            auto lColor = aContext->mColor;
            auto lData = aContext->mData;

            for (unsigned int y = 0; y < aContext->mBoxSizeY_px; y++)
            {
                for (unsigned int x = 0; x < aContext->mBoxSizeX_byte; x++)
                {
                    lData[x] = lColor[x % PIXEL_SIZE_byte];
                }

                lData += aContext->mSizeX_byte;
            }
        }

        void Bitmap::SetBox_DIV(SetBoxContext* aContext)
        {
            auto lColor = aContext->mColor;
            auto lData = aContext->mData;

            for (unsigned int y = 0; y < aContext->mBoxSizeY_px; y++)
            {
                for (unsigned int x = 0; x < aContext->mBoxSizeX_byte; x++)
                {
                    lData[x] = Saturated::Div(lData[x], lColor[x % PIXEL_SIZE_byte]);
                }

                lData += aContext->mSizeX_byte;
            }
        }

        void Bitmap::SetBox_MULT(SetBoxContext* aContext)
        {
            auto lColor = aContext->mColor;
            auto lData = aContext->mData;

            for (unsigned int y = 0; y < aContext->mBoxSizeY_px; y++)
            {
                for (unsigned int x = 0; x < aContext->mBoxSizeX_byte; x++)
                {
                    lData[x] = Saturated::Mul(lData[x], lColor[x % PIXEL_SIZE_byte]);
                }

                lData += aContext->mSizeX_byte;
            }
        }

        void Bitmap::SetBox_OR(SetBoxContext* aContext)
        {
            auto lColor = aContext->mColor;
            auto lData = aContext->mData;

            for (unsigned int y = 0; y < aContext->mBoxSizeY_px; y++)
            {
                for (unsigned int x = 0; x < aContext->mBoxSizeX_byte; x++)
                {
                    lData[x] |= lColor[x % PIXEL_SIZE_byte];
                }

                lData += aContext->mSizeX_byte;
            }
        }

        void Bitmap::SetBox_SUB(SetBoxContext* aContext)
        {
            auto lColor = aContext->mColor;
            auto lData  = aContext->mData;

            for (unsigned int y = 0; y < aContext->mBoxSizeY_px; y++)
            {
                for (unsigned int x = 0; x < aContext->mBoxSizeX_byte; x++)
                {
                    lData[x] = Saturated::Sub(lData[x], lColor[x % PIXEL_SIZE_byte]);
                }

                lData += aContext->mSizeX_byte;
            }
        }

        void Bitmap::SetBox_XOR(SetBoxContext* aContext)
        {
            auto lColor = aContext->mColor;
            auto lData = aContext->mData;

            for (unsigned int y = 0; y < aContext->mBoxSizeY_px; y++)
            {
                for (unsigned int x = 0; x < aContext->mBoxSizeX_byte; x++)
                {
                    lData[x] ^= lColor[x % PIXEL_SIZE_byte];
                }

                lData += aContext->mSizeX_byte;
            }
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
