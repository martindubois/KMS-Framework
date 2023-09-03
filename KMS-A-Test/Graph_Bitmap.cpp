
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Graph_Bitmap.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Graph/Bitmap.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define SIZE_X_px (32U)
#define SIZE_Y_px (64U)

KMS_TEST(Graph_Bitmap_Main, "Graph_Bitmap_Base", "Auto", sTest_Base)
{
    // Constructor
    Graph::Bitmap lB0;

    // Init
    lB0.Init(SIZE_X_px, SIZE_Y_px);

    // GetPixel
    KMS_TEST_ASSERT(Graph::Color::BLACK == lB0.GetPixel(Graph::Point::ORIGIN));

    // GetSizeX_px
    KMS_TEST_COMPARE(lB0.GetSizeX_px(), SIZE_X_px);

    // GetSizeY_px
    KMS_TEST_COMPARE(lB0.GetSizeY_px(), SIZE_Y_px);

    // SetBox
    lB0.SetBox(Graph::Point(2, 2), Graph::Point(SIZE_X_px / 2 - 3, SIZE_Y_px / 2 - 3), Graph::Color::WHITE);

    // SetLine
    lB0.SetLine(Graph::Point( 4,  4), Graph::Point(SIZE_X_px - 3, SIZE_Y_px - 2), Graph::Color::RED, Graph::Bitmap::Operation::OP_XOR);
    lB0.SetLine(Graph::Point( 1, 48), Graph::Point(SIZE_X_px - 2,            48),          0x101010, Graph::Bitmap::Operation::OP_ADD);
    lB0.SetLine(Graph::Point(22,  2), Graph::Point(           22, SIZE_Y_px - 2),          0x101010, Graph::Bitmap::Operation::OP_SUB);
    lB0.SetLine(Graph::Point( 4, 48), Graph::Point(SIZE_X_px - 3,             4),          0x3f3f3f, Graph::Bitmap::Operation::OP_AND);

    // SetPixel
    lB0.SetPixel(Graph::Point::ORIGIN, Graph::Color::RED);
    lB0.SetPixel(Graph::Point(0, 1),            0x804020, Graph::Bitmap::Operation::OP_ADD);
    lB0.SetPixel(Graph::Point(0, 1), Graph::Color::RED  , Graph::Bitmap::Operation::OP_AND);
    lB0.SetPixel(Graph::Point(0, 2), Graph::Color::GREEN, Graph::Bitmap::Operation::OP_COPY);
    lB0.SetPixel(Graph::Point(0, 3), Graph::Color::GREEN, Graph::Bitmap::Operation::OP_OR);
    lB0.SetPixel(Graph::Point(0, 3),            0x804020, Graph::Bitmap::Operation::OP_SUB);
    lB0.SetPixel(Graph::Point(0, 4), Graph::Color::BLUE , Graph::Bitmap::Operation::OP_XOR);

    // Scroll_Left
    lB0.Scroll_Left(1);

    // Scroll_Right
    lB0.Scroll_Right(1);
}
