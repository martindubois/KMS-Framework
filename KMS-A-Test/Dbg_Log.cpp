
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Dbg_Log.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Redirection.h>
#include <KMS/Dbg/Log.h>
#include <KMS/Dbg/Log_DI2.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

using namespace KMS;

// We could use Dbg::Log::SetHideCount to hide error message on console, but
// doing that would not thest the code displaying error on the console.

KMS_TEST(Dbg_Log_Base, "Auto", sTest_Base)
{
    Dbg::Log_Config lCfg;

    // Constructor
    Dbg::Log  lL;

    // SetHideCount
    Exception lE(__FILE__, __FUNCTION__, __LINE__, RESULT_ERROR, "Test");

    // CloseLogFiles
    Console::Redirection lR(Console::Redirection::What::WHAT_STDERR);
    {
        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_ERROR);
    }
    lR.Restore();
    lL.WriteData(&lL, sizeof(lL));
    lR.Redirect();
    {
        lL.WriteException(lE);
        lL.WriteMessage("Test");

        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_WARNING);
    }
    lR.Restore();
    lL.WriteData(&lL, sizeof(lL));
    lR.Redirect();
    {
        lL.WriteMessage("Test");
    }
    lR.Restore();

    lL.CloseLogFiles();

    // IsFileEnabled *
    KMS_TEST_ASSERT(!lL.IsFileEnabled());

    // WriteEntry

    // WriteMessage

    // WriteData

    // WriteException

    // WriteVersion

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_NOISE);
    lL.WriteData(&lL, sizeof(lL));

    lR.Redirect();
    {
        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_WARNING);
    }
    lR.Restore();
    lL.WriteData(&lL, sizeof(lL));
    lR.Redirect();
    {
        lL.WriteException(lE);

        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_ERROR);
    }
    lR.Restore();
    lL.WriteData(&lL, sizeof(lL));
    lR.Redirect();
    {
        lL.WriteException(lE);
        lL.WriteMessage("Test");
    }
    lR.Restore();

    lL.Config_Get(&lCfg);

    lCfg.SetConsoleMode(Dbg::ConsoleMode::MODE_DEBUG);

    lL.Config_Set(lCfg);

    lR.Redirect();
    {
        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_WARNING);
        lL.WriteData(&lL, sizeof(lL));
        lL.WriteException(lE);

        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_ERROR);
        lL.WriteData(&lL, sizeof(lL));
        lL.WriteException(lE);
        lL.WriteMessage("Test");

        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_WARNING);
        lL.WriteData(&lL, sizeof(lL));
    }
    lR.Restore();

    KMS_DBG_LOG_INFO();
    Dbg::gLog.WriteData(&lL, sizeof(lL));
}

KMS_TEST(Dbg_Log_DI2, "Auto", sTest_DI2)
{
    char lBuffer[LINE_LENGTH];
    Dbg::Log_Config lCfg;

    DI2::Code_ASCII_String(&lCfg, &Dbg::Log_Config_TYPE, sizeof(lBuffer), lBuffer);
    DI2::Decode_ASCII_String(&lCfg, &Dbg::Log_Config_TYPE, lBuffer);
}

KMS_TEST(Dbg_Log_File, "Auto", sTest_File)
{
    Dbg::Log_Config lCfg;

    lCfg.SetConsoleLevel(Dbg::Level::LEVEL_NOISE);
    lCfg.SetConsoleMode(Dbg::ConsoleMode::MODE_USER);
    lCfg.SetFolder(".");

    // Constructor
    Dbg::Log  lL;

    lL.Config_Set(lCfg);

    // IsFileEnabled
    KMS_TEST_ASSERT(lL.IsFileEnabled());

    // CloseLogFiles
    lL.CloseLogFiles();

    // WriteEntry
    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_ERROR);

    // WriteData
    lL.WriteData("Data", 4);

    // WriteMessage
    lL.WriteMessage("Message");

    // WriteException
    Exception lE(__FILE__, __FUNCTION__, __LINE__, RESULT_ERROR, "Test exception logging");
    lL.WriteException(lE);

    lCfg.SetConsoleMode(Dbg::ConsoleMode::MODE_DEBUG);

    lL.Config_Set(lCfg);

    // WriteEntry
    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_NOISE);
    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::Level::LEVEL_INFO);

    // WriteData
    lL.WriteData("Data", 4);

    // WriteException
    lL.WriteException(lE);

    // WriteMessage
    lL.WriteMessage("Message");

    lCfg.SetFolder(".gitignore");

    try
    {
        lL.Config_Set(lCfg);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_CONFIG);

    // Destructor
}
