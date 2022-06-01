
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Test_TestManager.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/Test/TestManager.h>

KMS_TEST(Test_TestManager_Fail, "Test_TestManager_Fail", "Auto", sTest_Fail)
{
    KMS::Config::Configurator lC;
    KMS::Test::TestManager lTM;

    lTM.InitConfigurator(&lC);

    lC.Init();

    const char* lVector[5] =
    {
        "Test+=Test_TestManager_Fail0",
        "Test+=Test_TestManager_Fail1",
        "Test+=Test_TestManager_Fail2",
        "Test+=Test_TestManager_Fail3",
        "Unknown+=0"
    };

    lC.ParseArguments(5, lVector);

    KMS_TEST_COMPARE(lC.GetIgnoredCount(), 1U);

    std::cerr << KMS::Console::Color::BLUE;
    std::cerr << "IMPORTANT : Ignore the following error messages. The current test tests failling tests." << std::endl;
    std::cerr << KMS::Console::Color::WHITE;

    KMS_TEST_COMPARE(lTM.Run(), 98);

    std::cout << lTM;

    std::cerr << KMS::Console::Color::BLUE;
    std::cerr << "IMPORTANT : End of the test of failling tests." << std::endl;
    std::cerr << KMS::Console::Color::WHITE;

    KMS_TEST_COMPARE(lTM.GetErrorCount(), 6U);
}

// Failing tests
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(Test_TestManager_Fail0, "Test_TestManager_Fail0", "Fail", sTest_Fail0)
{
    KMS_TEST_ASSERT(false);
    KMS_TEST_COMPARE(0, 1);
    KMS_TEST_COMPARE(0U, 1U);
}

KMS_TEST(Test_TestManager_Fail1, "Test_TestManager_Fail1", "Fail", sTest_Fail1)
{
    KMS_EXCEPTION(TEST, "Test");
}

KMS_TEST(Test_TestManager_Fail2, "Test_TestManager_Fail2", "Fai2", sTest_Fail2)
{
    throw std::exception("Test");
}

KMS_TEST(Test_TestManager_Fail3, "Test_TestManager_Fail3", "Fai3", sTest_Fail3)
{
    throw "Test";
}
