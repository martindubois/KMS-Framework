
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/ByteTool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/ByteTool.h>

KMS_TEST(ByteTool, "ByteTool", "Auto", sTest)
{
    static const char* VECTOR[] =
    {
        "Source=ByteTool.cpp",
        "Destination=ByteTool.cpp.bak",
        "ByteTable[0]=0",
        "DoesNotExist=Ignored",
        "DoesNotExist+=Ignored",
        "DoesNotExist[0]=Ignored",
    };

    KMS::ByteTool             lBT;
    KMS::Config::Configurator lC;

    lBT.InitConfigurator(&lC);

    lC.Init();
    lC.ParseArguments(6, VECTOR);

    KMS_TEST_COMPARE(lC.GetIgnoredCount(), 3U);

    KMS_TEST_COMPARE(lBT.Run(), 0);

    const unsigned int* lH = lBT.GetHistrogram();
    KMS_TEST_ASSERT(NULL != lH);

    KMS_TEST_COMPARE(lH[0], 0U);
}

KMS_TEST(ByteTool_Fail, "ByteTool_Fail", "Auto", sTest_Fail)
{
    static const char* VECTOR[] =
    {
        "Source=ByteTool.cpp",
        "Destination=ByteTool.cpp.bak",
        "ByteTable[0]=0",
        "DoesNotExist=Ignored",
        "DoesNotExist+=Ignored",
        "DoesNotExist[0]=Ignored",
        "ByteTable[256]=0",
    };

    KMS::ByteTool             lBT;
    KMS::Config::Configurator lC;

    lBT.InitConfigurator(&lC);

    lC.Init();

    try
    {
        lC.ParseArguments(7, VECTOR);

        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONFIG_INDEX);
}

KMS_TEST(ByteTool_Main, "ByteTool_Main", "Auto", sTest_Main)
{
    static const char* VECTOR[] =
    {
        "KMS-A-Test.exe",
        "Source=ByteTool.cpp",
    };

    KMS::ByteTool::Main(2, VECTOR);
}
