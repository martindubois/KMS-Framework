
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Dbg_Log.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dbg/Log.h>

using namespace KMS;

// We could use Dbg::Log::SetHideCount to hide error message on console, but
// doing that would not thest the code displaying error on the console.

KMS_TEST(Dbg_Log_Base, "Auto", sTest_Base)
{
    // Constructor
    Dbg::Log  lL;
    lL.mConsole.Set_Null();

    // SetHideCount
    Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
    Exception lE(__FILE__, __FUNCTION__, __LINE__, RESULT_ERROR, "Test");

    // CloseLogFiles

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_ERROR);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);
    lL.WriteMessage("Test");

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteMessage("Test");

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

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_ERROR);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);
    lL.WriteMessage("Test");

    lL.mConsoleMode = Dbg::Log::ConsoleMode::MODE_DEBUG;

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_ERROR);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);
    lL.WriteMessage("Test");

    lL.SetHideCount(Dbg::LogFile::Level::LEVEL_WARNING, 2);

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
    lL.WriteData(&lL, sizeof(lL));

    KMS_DBG_LOG_INFO();
    Dbg::gLog.WriteData(&lL, sizeof(lL));
}
