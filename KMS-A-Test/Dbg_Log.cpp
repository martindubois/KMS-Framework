
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Dbg_Log.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dbg/Log.h>

KMS_TEST(Dbg_Log_Base, "Dbg_Log_Base", "Auto", sTest_Base)
{
    KMS_TEST_EXPECTED_EXCEPTION();
    KMS::Exception lE(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::TEST, "Test");
    KMS::Dbg::Log  lL;

    lL.mFolder.Set("DoesNotExist");

    lL.CloseLogFiles();

    KMS_TEST_ASSERT(!lL.IsFileEnabled());

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_NOISE);
    lL.WriteData(&lL, sizeof(lL));

    KMS_TEST_EXPECTED_WARNINGS(1);
    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_WARNING);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);

    KMS_TEST_EXPECTED_ERROR();
    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, KMS::Dbg::LogFile::Level::LEVEL_ERROR);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);
    lL.WriteMessage("Test");

    lL.mConsoleLevel.Set("NONE");
    lL.mConsoleLevel.Set("ERROR");
    lL.mConsoleLevel.Set("WARNING");
    lL.mConsoleLevel.Set("INFO");
    lL.mConsoleLevel.Set("NOISE");

    lL.mFileLevel.Set("NONE");
    lL.mFileLevel.Set("ERROR");
    lL.mFileLevel.Set("WARNING");
    lL.mFileLevel.Set("INFO");
    lL.mFileLevel.Set("NOISE");

    KMS_DBG_LOG_INFO();
    KMS::Dbg::gLog.WriteData(&lL, sizeof(lL));
}
