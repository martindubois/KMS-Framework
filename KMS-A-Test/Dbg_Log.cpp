
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Dbg_Log.cpp

// TEST COVERAGE 2022-08-15 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/Dbg/Log.h>

KMS_TEST(Dbg_Log_Base, "Dbg_Log_Base", "Auto", sTest_Base)
{
    KMS_TEST_EXPECTED_EXCEPTION();
    KMS::Exception lE(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::TEST, "Test");
    KMS::Dbg::Log  lL;

    lL.DisplayHelp(stdout);

    lL.SetAttribute("Folder", "DoesNotExist");

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

    KMS_TEST_ASSERT(lL.SetAttribute("ConsoleLevel", "NONE"));
    KMS_TEST_ASSERT(lL.SetAttribute("ConsoleLevel", "ERROR"));
    KMS_TEST_ASSERT(lL.SetAttribute("ConsoleLevel", "WARNING"));
    KMS_TEST_ASSERT(lL.SetAttribute("ConsoleLevel", "INFO"));
    KMS_TEST_ASSERT(lL.SetAttribute("ConsoleLevel", "NOISE"));

    KMS_TEST_ASSERT(lL.SetAttribute("ConsoleLevel", NULL));

    KMS_TEST_ASSERT(lL.SetAttribute("FileLevel", "NONE"   ));
    KMS_TEST_ASSERT(lL.SetAttribute("FileLevel", "ERROR"  ));
    KMS_TEST_ASSERT(lL.SetAttribute("FileLevel", "WARNING"));
    KMS_TEST_ASSERT(lL.SetAttribute("FileLevel", "INFO"   ));
    KMS_TEST_ASSERT(lL.SetAttribute("FileLevel", "NOISE"  ));

    KMS_TEST_ASSERT(lL.SetAttribute("FileLevel", NULL));

    KMS_TEST_ASSERT(!lL.SetAttribute("Invalid", NULL));

    KMS_DBG_LOG_INFO();
    KMS::Dbg::gLog.WriteData(&lL, sizeof(lL));
}

KMS_TEST(Dbg_Log_Exception, "Dbg_Log_Exception", "Auto", sTest_Exception)
{
    KMS::Dbg::Log  lL;

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lL.SetAttribute("FileLevel", "INVALID");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONFIG_VALUE);
}
