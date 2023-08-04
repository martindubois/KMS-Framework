
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

using namespace KMS;

KMS_TEST(Test_TestManager_Fail, "Test_TestManager_Fail", "Auto", sTest_Fail)
{
    Cfg::Configurator      lC;
    KMS::Test::TestManager lTM;

    lTM.mConsole.Set_Null();

    lC.AddConfigurable(&lTM);

    const char* lVector[5] =
    {
        "Tests+=Test_TestManager_Fail0",
        "Tests+=Test_TestManager_Fail1",
        "Tests+=Test_TestManager_Fail2",
        "Tests+=Test_TestManager_Fail3",
        "Unknown+=0"
    };

    Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_WARNING, 2);
    lC.ParseArguments(5, lVector);

    KMS_TEST_COMPARE(lC.GetIgnoredCount(), 1U);

    KMS_TEST_COMPARE(lTM.Run(), 98);

    lTM.mConsole.OutputStream() << lTM;

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
    Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
    KMS_EXCEPTION(TEST, "Test", "");
}

KMS_TEST(Test_TestManager_Fail2, "Test_TestManager_Fail2", "Fai2", sTest_Fail2)
{
    throw std::exception();
}

KMS_TEST(Test_TestManager_Fail3, "Test_TestManager_Fail3", "Fai3", sTest_Fail3)
{
    throw "Test";
}
