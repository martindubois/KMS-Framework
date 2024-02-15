
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Dbg_Log.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Redirection.h>
#include <KMS/Dbg/Log.h>

using namespace KMS;

// We could use Dbg::Log::SetHideCount to hide error message on console, but
// doing that would not thest the code displaying error on the console.

KMS_TEST(Dbg_Log_Base, "Auto", sTest_Base)
{
    // Constructor
    Dbg::Log  lL;

    // SetHideCount
    Exception lE(__FILE__, __FUNCTION__, __LINE__, RESULT_ERROR, "Test");

    // CloseLogFiles
    Console::Redirection lR(Console::Redirection::What::WHAT_STDERR);
    {
        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_ERROR);
    }
    lR.Restore();
    lL.WriteData(&lL, sizeof(lL));
    lR.Redirect();
    {
        lL.WriteException(lE);
        lL.WriteMessage("Test");

        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
    }
    lR.Restore();
    lL.WriteData(&lL, sizeof(lL));
    lR.Redirect();
    {
        lL.WriteMessage("Test");
    }
    lR.Restore();

    lL.mFolder = File::Folder("DoesNotExist");
    lL.CloseLogFiles();

    // IsFileEnabled *
    KMS_TEST_ASSERT(!lL.IsFileEnabled());

    // WriteEntry

    // WriteMessage

    // WriteData

    // WriteException

    // WriteVersion


    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_NOISE);
    lL.WriteData(&lL, sizeof(lL));

    lR.Redirect();
    {
        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
    }
    lR.Restore();
    lL.WriteData(&lL, sizeof(lL));
    lR.Redirect();
    {
        lL.WriteException(lE);

        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_ERROR);
    }
    lR.Restore();
    lL.WriteData(&lL, sizeof(lL));
    lR.Redirect();
    {
        lL.WriteException(lE);
        lL.WriteMessage("Test");
    }
    lR.Restore();

    lL.mConsoleMode = Dbg::Log::ConsoleMode::MODE_DEBUG;

    lR.Redirect();
    {
        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
        lL.WriteData(&lL, sizeof(lL));
        lL.WriteException(lE);

        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_ERROR);
        lL.WriteData(&lL, sizeof(lL));
        lL.WriteException(lE);
        lL.WriteMessage("Test");

        lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
        lL.WriteData(&lL, sizeof(lL));
    }
    lR.Restore();

    KMS_DBG_LOG_INFO();
    Dbg::gLog.WriteData(&lL, sizeof(lL));
}
