
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/CLI_Tool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/Tool.h>
#include <KMS/Console/Redirection.h>

using namespace KMS;

class Test_Tool : public CLI::Tool
{
};

// Constants
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(CLI_Tool_Exception, "Auto", sTest_Exception)
{
    Test_Tool lT;

    // ExecuteCommand

    CLI::CommandLine lCL0("Invalid");

    try
    {
        lT.ExecuteCommand(&lCL0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_COMMAND);
}
