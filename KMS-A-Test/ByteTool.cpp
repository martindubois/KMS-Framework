
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/ByteTool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/ByteTool.h>

KMS_TEST(ByteTool_Base, "ByteTool_Base", "Auto", sTest_Base)
{
    static const char* VECTOR[] =
    {
        "Source=KMS-A-Test" SLASH "ByteTool.cpp",
        "DoesNotExist=Ignored",
        "DoesNotExist+=Ignored",
        "OptionalConfigFile+=DoesNotExist.cfg",
        "DoesNotExist[0]=Ignored",
        "DoesNotExist",
        "SaveConfig=Test.txt",
    };

    KMS::ByteTool          lBT;
    KMS::Cfg::Configurator lC;

    lC.AddConfigurable(&lBT);

    lC.Help();

    KMS_TEST_EXPECTED_WARNINGS(4);
    lC.ParseArguments(7, VECTOR);

    KMS_TEST_COMPARE(lC.GetIgnoredCount(), 4U);

    KMS_TEST_COMPARE(lBT.Run(), 0);

    const unsigned int* lH = lBT.GetHistrogram();
    KMS_TEST_ASSERT(NULL != lH);

    KMS_TEST_COMPARE(lH[0], 0U);
}

KMS_TEST(ByteTool_Main, "ByteTool_Main", "Auto", sTest_Main)
{
    static const char* VECTOR[] =
    {
        "KMS-A-Test.exe",
        "Source=KMS-A-Test" SLASH "ByteTool.cpp",
    };

    KMS::ByteTool::Main(2, VECTOR);
}
