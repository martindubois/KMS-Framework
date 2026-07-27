
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C-Test/Dev_Device2.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dev/Device2.h>
#include <KMS/Dev/Device2_DI2.h>
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Output.h>

using namespace KMS;

KMS_TEST(Dev_Device2_Config, "Auto", sTest_Config)
{
    Dev::Device2_Config lCfg0;

    // IsInterfaceValid - Windows only

    // IsLinkValid
    KMS_TEST_ASSERT(!lCfg0.IsLinkValid());

    // IsValid
    KMS_TEST_ASSERT(!lCfg0.IsValid());

    // LinkFromInterfaceAndIndex - Linux only

    // SetDefault
    lCfg0.SetDefault();

    // SetLink
    lCfg0.SetLink("InvalidLink");

    // Validate
    lCfg0.Validate();
}

KMS_TEST(Dev_Device2_Config_DI2, "Auto", sTest_Config_DI2)
{
    char lBuffer[4096];

    Dev::Device2_Config lCfg0;
    Dev::Device2_Config lCfg1;

    DI2::Code_ASCII_String(&lCfg0, &Dev::Device2_Config_TYPE, sizeof(lBuffer), lBuffer);

    DI2::Decode_ASCII_String(&lCfg1, &Dev::Device2_Config_TYPE, lBuffer);

    KMS_TEST_ASSERT(0 == memcmp(&lCfg0, &lCfg1, sizeof(lCfg0)));
}

KMS_TEST(Dev_Device2_Config_Exception, "Auto", sTest_Config_Exception)
{
    Dev::Device2_Config lCfg0;

    try
    {
        lCfg0.Validate();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_CONFIG);
}

KMS_TEST(Dev_Device2_Exception, "Auto", sTest_Exception)
{
    Dev::Device2 lD0;

    try
    {
        lD0.Connect();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_CONFIG);
}
