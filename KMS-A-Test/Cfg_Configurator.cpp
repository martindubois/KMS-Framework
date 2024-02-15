
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Cfg_Configurator.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Console/Redirection.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

static const char* SILENCED[] = { "Silenced", nullptr };

KMS_TEST(Cfg_Configurator_Base, "Auto", sTest_Base)
{
    // Constructor
    Cfg::Configurator lC;

    // AddConfigurable

    // GetIgnoredCount

    // SetSilence
    lC.SetSilence(SILENCED);

    // AddConfigFile *
    Console::Redirection lR(Console::Redirection::What::WHAT_STDOUT);
    {
        lC.AddConfigFile("KMS-A-Test/Tests/Test1.cfg");
    }
    lR.Restore();

    // AddOptionalConfigFile *
    lC.AddOptionalConfigFile("DoesNotExist.cfg");

    // DisplayConfig
    lR.Redirect();
    {
        lC.DisplayConfig();
    }
    lR.Restore();

    // Help
    lR.Redirect();
    {
        lC.Help(stdout);
    }
    lR.Restore();

    // ParseArguments

    // ParseFile

    // SaveConfig
    lC.SaveConfig("");

    // Validate
    lC.Validate();
}
