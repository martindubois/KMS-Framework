
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

static const char* SILENCED[] = { "Silenced", NULL };

KMS_TEST(Cfg_Configurator_Base, "Cfg_Configurator_Base", "Auto", sTest_Base)
{
    Cfg::Configurator lC;

    lC.mConsole.Set_Null();

    lC.SetSilence(SILENCED);

    Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_WARNING, 6);
    lC.AddConfigFile("KMS-A-Test/Tests/Test1.cfg");

    lC.AddOptionalConfigFile("DoesNotExist.cfg");

    lC.DisplayConfig();

    lC.SaveConfig("");
}
