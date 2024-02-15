
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Scope_Scope.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Scope/Channel_Random.h>
#include <KMS/Scope/Scope.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define SIZE_X_px (512)
#define SIZE_Y_px (512)

KMS_TEST(Scope_Scope_Base, "Auto", sTest_Base)
{
    srand(0);

    // Constructor
    Scope::Scope lS0;
    lS0.mBitmap.Init(SIZE_X_px, SIZE_Y_px);

    // Clear
    lS0.Clear();

    // ForceTrig
    lS0.ForceTrig();

    // Sample
    lS0.Sample();

    // Start
    lS0.Start();

    // ForceTrig - When waiting
    lS0.ForceTrig();

    // Stop
    lS0.Stop();

    // Channel_Add
    Scope::Channel_Random lC0;
    lS0.Channel_Add(&lC0);

    // Sample - With channel
    lS0.Sample();

    // Channel_Remove
    lS0.Channel_Remove(&lC0);

    // ===== Full frame =====================================================
    FileFormat::BMP lF1(File::Folder::CURRENT, "Test_Scope_Scope_Base_1.bmp", true);
    lF1.Create(SIZE_X_px, SIZE_Y_px);
    Scope::Scope lS1;
    lS1.mFrequency_Hz = 1000;
    lS1.mBitmap.Init(&lF1);
    lS1.Channel_Add(&lC0);
    lS1.Start();
    for (unsigned int i = 0; i < SIZE_X_px * 2; i++)
    {
        lS1.Sample();
    }
    lS1.Channel_Remove(&lC0);

    FileFormat::BMP lF2(File::Folder::CURRENT, "Test_Scope_Scope_Base_2.bmp", true);
    lF2.Create(SIZE_X_px, SIZE_Y_px);
    Scope::Scope lS2;
    lS2.mFrequency_Hz = 1000;
    lS2.mScaleX_ms_px = 200;
    lS2.mBitmap.Init(&lF2);
    lS2.Channel_Add(&lC0);
    lS2.Start();
    for (unsigned int i = 0; i < SIZE_X_px * 3; i++)
    {
        lS2.Sample();
    }
    lS2.Channel_Remove(&lC0);

    FileFormat::BMP lF3(File::Folder::CURRENT, "Test_Scope_Scope_Base_3.bmp", true);
    lF3.Create(SIZE_X_px, SIZE_Y_px);
    Scope::Scope lS3;
    lS3.mFrequency_Hz = 1000;
    lS3.mMode = Scope::Scope::Mode::CONTINUOUS;
    lS3.mScaleX_ms_px = 200;
    lS3.mBitmap.Init(&lF3);
    lS3.Channel_Add(&lC0);
    lS3.Start();
    for (unsigned int i = 0; i < SIZE_X_px * 3; i++)
    {
        lS3.Sample();
    }
    lS3.Channel_Remove(&lC0);

    FileFormat::BMP lF4(File::Folder::CURRENT, "Test_Scope_Scope_Base_4.bmp", true);
    lF4.Create(SIZE_X_px, SIZE_Y_px);
    Scope::Scope lS4;
    lS4.mFrequency_Hz = 1000;
    lS4.mMode = Scope::Scope::Mode::SINGLE;
    lS4.mScaleX_ms_px = 200;
    lS4.mBitmap.Init(&lF4);
    lS4.Channel_Add(&lC0);
    lS4.Start();
    for (unsigned int i = 0; i < SIZE_X_px * 3; i++)
    {
        lS4.Sample();
    }
    lS4.Channel_Remove(&lC0);

    FileFormat::BMP lF5(File::Folder::CURRENT, "Test_Scope_Scope_Base_5.bmp", true);
    lF5.Create(SIZE_X_px, SIZE_Y_px);
    Scope::Scope lS5;
    lS5.mFrequency_Hz = 1000;
    lS5.mMode = Scope::Scope::Mode::SINGLE;
    lS5.mPositionX_px = 2;
    lS5.mScaleX_ms_px = 200;
    lS5.mBitmap.Init(&lF5);
    lS5.Channel_Add(&lC0);
    lS5.Start();
    for (unsigned int i = 0; i < SIZE_X_px * 3; i++)
    {
        lS5.Sample();
    }
    lS5.Channel_Remove(&lC0);

    FileFormat::BMP lF6(File::Folder::CURRENT, "Test_Scope_Scope_Base_6.bmp", true);
    lF6.Create(SIZE_X_px, SIZE_Y_px);
    Scope::Scope lS6;
    lS6.mFrequency_Hz = 1000;
    lS6.mMode = Scope::Scope::Mode::AUTO;
    lS6.mPositionX_px = 2;
    lS6.mScaleX_ms_px = 200;
    lC0.mTrigLevel = 1000.0;
    lS6.mBitmap.Init(&lF6);
    lS6.Channel_Add(&lC0);
    lS6.Start();
    for (unsigned int i = 0; i < SIZE_X_px * 4; i++)
    {
        lS6.Sample();
    }
    lS6.Channel_Remove(&lC0);

    // ===== Cleanup ========================================================
    lF1.Close();
    lF2.Close();
    lF3.Close();
    lF4.Close();
    lF5.Close();
    lF6.Close();
    File::Folder::CURRENT.Delete("Test_Scope_Scope_Base_1.bmp");
    File::Folder::CURRENT.Delete("Test_Scope_Scope_Base_2.bmp");
    File::Folder::CURRENT.Delete("Test_Scope_Scope_Base_3.bmp");
    File::Folder::CURRENT.Delete("Test_Scope_Scope_Base_4.bmp");
    File::Folder::CURRENT.Delete("Test_Scope_Scope_Base_5.bmp");
    File::Folder::CURRENT.Delete("Test_Scope_Scope_Base_6.bmp");
}
