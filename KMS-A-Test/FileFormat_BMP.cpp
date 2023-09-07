
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/FileFormat_BMP.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/FileFormat/BMP.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define BYTE_PER_PIXEL (3)

#define SIZE_X (64U)
#define SIZE_Y (32U)

KMS_TEST(FileFormat_BMP_Main, "Auto", sTest_Base)
{
    // Constructeur
    FileFormat::BMP lBMP0(File::Folder::CURRENT, "KMS-A-Test/Tests/Test_FileFormat_BMP_Base_0.bmp");
    FileFormat::BMP lBMP1(File::Folder::CURRENT, "Test_FileFormat_BMP_Base_1.bmp", true);

    // GetData
    KMS_TEST_ASSERT(nullptr == lBMP0.GetData());

    // GetDataSize_byte

    // GetHeader
    KMS_TEST_ASSERT(nullptr == lBMP0.GetHeader());

    // GetSize_byte

    // GetSizeX_px

    // GetSizeY_px

    // Close

    // Create
    lBMP1.Create(SIZE_X, SIZE_Y);

    // Open
    lBMP0.Open();


    // GetData *
    KMS_TEST_ASSERT(nullptr != lBMP0.GetData());

    // GetDataSize_byte *
    KMS_TEST_COMPARE(lBMP0.GetDataSize_byte(), static_cast<unsigned int>(SIZE_X * SIZE_Y * BYTE_PER_PIXEL));

    // GetHeader *
    KMS_TEST_ASSERT(nullptr != lBMP0.GetHeader());

    // GetSize_byte *
    KMS_TEST_COMPARE(lBMP0.GetSize_byte(), static_cast<unsigned int>(54 + SIZE_X * SIZE_Y * BYTE_PER_PIXEL));

    // GetSizeX_px *
    KMS_TEST_COMPARE(lBMP0.GetSizeX_px(), SIZE_X);

    // GetSizeY_px *
    KMS_TEST_COMPARE(lBMP0.GetSizeY_px(), SIZE_Y);

    // Close **
    lBMP1.Close();

    // ===== Cleanup ========================================================
    File::Folder::CURRENT.Delete("Test_FileFormat_BMP_Base_1.bmp");
}
