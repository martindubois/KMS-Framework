
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Cfg_Configurator.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

static const char* SILENCED[] = { "Silenced", nullptr };

KMS_TEST(Cfg_Configurator_Base, "Auto", sTest_Base)
{
    // Constructor
    Cfg::Configurator lC;

    lC.mConsole.Set_Null();

    // AddConfigurable

    // GetIgnoredCount

    // SetSilence
    lC.SetSilence(SILENCED);

    // AddConfigFile *
    lC.AddConfigFile("KMS-A-Test/Tests/Test1.cfg");

    // AddOptionalConfigFile *
    lC.AddOptionalConfigFile("DoesNotExist.cfg");

    // DisplayConfig
    lC.DisplayConfig();

    // Help
    lC.Help(lC.mConsole.OutputFile());

    // ParseArguments

    // ParseFile

    // SaveConfig
    lC.SaveConfig("");

    // Validate
    lC.Validate();
}
