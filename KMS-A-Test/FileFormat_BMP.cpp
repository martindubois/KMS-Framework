
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/FileFormat_ELF.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/FileFormat/BMP.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define SIZE_X (32)
#define SIZE_Y (64)

KMS_TEST(FileFormat_BMP_Main, "FileFormat_BMP_Base", "Auto", sTest_Base)
{
    // MakeColor

    // Constructeur
    FileFormat::BMP lBMP0(File::Folder::CURRENT, "KMS-A-Test/Tests/Test_FileFormat_BMP_Base_0.bmp");
    FileFormat::BMP lBMP1(File::Folder::CURRENT, "Test_FileFormat_BMP_Base_1.bmp", true);

    // GetData
    KMS_TEST_ASSERT(nullptr == lBMP0.GetData());

    // GetDataSize_byte

    // GetHeader
    KMS_TEST_ASSERT(nullptr == lBMP0.GetHeader());

    // GetPixel

    // GetSize_byte

    // GetSizeX_px

    // GetSizeY_px

    // SetBox

    // SetLine

    // SetPixel

    // Close

    // Create
    lBMP1.Create(SIZE_X, SIZE_Y);

    // Open
    lBMP0.Open();


    // GetData *
    KMS_TEST_ASSERT(nullptr != lBMP0.GetData());

    // GetDataSize_byte *
    KMS_TEST_COMPARE(lBMP0.GetDataSize_byte(), static_cast<unsigned int>(32 * 64 * 3));

    // GetHeader *
    KMS_TEST_ASSERT(nullptr != lBMP0.GetHeader());

    // GetPixel *
    KMS_TEST_ASSERT(FileFormat::BMP::BLUE  == lBMP0.GetPixel( 0,  0));
    KMS_TEST_ASSERT(FileFormat::BMP::RED   == lBMP0.GetPixel(63,  0));
    KMS_TEST_ASSERT(FileFormat::BMP::WHITE == lBMP0.GetPixel( 0, 31));

    // GetSize_byte *
    KMS_TEST_COMPARE(lBMP0.GetSize_byte(), static_cast<unsigned int>(54 + 32 * 64 * 3));

    // GetSizeX_px *
    KMS_TEST_COMPARE(lBMP0.GetSizeX_px(), 64U);

    // GetSizeY_px *
    KMS_TEST_COMPARE(lBMP0.GetSizeY_px(), 32U);

    // SetBox *
    lBMP1.SetBox(2, 2, SIZE_Y / 2 - 3, SIZE_Y / 2 - 3, FileFormat::BMP::WHITE);

    // SetLine *
    lBMP1.SetLine( 4,  4, SIZE_X - 3, SIZE_Y - 2, FileFormat::BMP::RED, FileFormat::BMP::Operation::OP_XOR);
    lBMP1.SetLine( 1, 48, SIZE_X - 2,         48,             0x101010, FileFormat::BMP::Operation::OP_ADD);
    lBMP1.SetLine(22,  2,         22, SIZE_Y - 2,             0x101010, FileFormat::BMP::Operation::OP_SUB);
    lBMP1.SetLine( 4, 48, SIZE_X - 3,          4,             0x3f3f3f, FileFormat::BMP::Operation::OP_AND);

    // SetPixel *
    lBMP1.SetPixel(0, 0, FileFormat::BMP::RED);
    lBMP1.SetPixel(0, 1,               0x804020, FileFormat::BMP::Operation::OP_ADD);
    lBMP1.SetPixel(0, 1, FileFormat::BMP::RED  , FileFormat::BMP::Operation::OP_AND);
    lBMP1.SetPixel(0, 2, FileFormat::BMP::GREEN, FileFormat::BMP::Operation::OP_COPY);
    lBMP1.SetPixel(0, 3, FileFormat::BMP::GREEN, FileFormat::BMP::Operation::OP_OR);
    lBMP1.SetPixel(0, 3,               0x804020, FileFormat::BMP::Operation::OP_SUB);
    lBMP1.SetPixel(0, 4, FileFormat::BMP::BLUE , FileFormat::BMP::Operation::OP_XOR);

    // Close **
    lBMP1.Close();

    // ===== Cleanup ========================================================
    File::Folder::CURRENT.Delete("Test_FileFormat_BMP_Base_1.bmp");
}
