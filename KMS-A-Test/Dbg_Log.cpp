
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

KMS_TEST(Dbg_Log_Base, "Dbg_Log_Base", "Auto", sTest_Base)
{
    Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
    Exception lE(__FILE__, __FUNCTION__, __LINE__, Exception::Code::TEST, "Test");
    Dbg::Log  lL;

    lL.mFolder = File::Folder("DoesNotExist");

    lL.CloseLogFiles();

    KMS_TEST_ASSERT(!lL.IsFileEnabled());

    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_NOISE);
    lL.WriteData(&lL, sizeof(lL));

    KMS_TEST_EXPECTED_WARNINGS(1);
    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_WARNING);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);

    KMS_TEST_EXPECTED_ERROR();
    lL.WriteEntry(__FILE__, __FUNCTION__, __LINE__, Dbg::LogFile::Level::LEVEL_ERROR);
    lL.WriteData(&lL, sizeof(lL));
    lL.WriteException(lE);
    lL.WriteMessage("Test");

    KMS_DBG_LOG_INFO();
    Dbg::gLog.WriteData(&lL, sizeof(lL));
}
