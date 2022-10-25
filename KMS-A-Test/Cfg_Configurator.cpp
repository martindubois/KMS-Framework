
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Cfg_Configurator.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>

KMS_TEST(Cfg_Configurator_Base, "Cfg_Configurator_Base", "Auto", sTest_Base)
{
    KMS::Cfg::Configurator lC;

    KMS_TEST_EXPECTED_WARNINGS(3);

    lC.AddConfigFile("KMS-A-Test/Tests/Test1.cfg");
}
