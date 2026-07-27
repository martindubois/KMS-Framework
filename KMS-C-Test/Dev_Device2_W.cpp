
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C-Test/Dev_Device2_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#define INITGUID
#include <Windows.h>
#include <Ndisguid.h>
#include <Ntddmou.h>
#include <Ntddkbd.h>

// ===== Includes ===========================================================
#include <KMS/Dev/Device2.h>
#include <KMS/Dev/Device2_DI2.h>
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Output.h>

using namespace KMS;

// {586DBACF-6F76-4448-85C4-DEF8DE99C438}
static const GUID INVALID_INTERFACE = { 0x586dbacf, 0x6f76, 0x4448, { 0x85, 0xc4, 0xde, 0xf8, 0xde, 0x99, 0xc4, 0x38 } };

KMS_TEST(Dev_Device2_Config_W, "Auto", sTest_Config_W)
{
    Dev::Device2_Config lCfg0;

    KMS_TEST_ASSERT(!lCfg0.IsInterfaceValid());

    lCfg0.mInterface = INVALID_INTERFACE;

    KMS_TEST_ASSERT(lCfg0.IsInterfaceValid());

    KMS_TEST_COMPARE(Dev::Device2::GetCount(lCfg0), 0L);

    lCfg0.mInterface = GUID_DEVINTERFACE_COMPORT;
    auto lComPort = Dev::Device2::GetCount(lCfg0);

    lCfg0.mInterface = GUID_DEVINTERFACE_KEYBOARD;
    auto lKeyboard = Dev::Device2::GetCount(lCfg0);

    lCfg0.mInterface = GUID_DEVINTERFACE_MOUSE;
    auto lMouse = Dev::Device2::GetCount(lCfg0);

    lCfg0.mInterface = GUID_DEVINTERFACE_NET;
    auto lNet = Dev::Device2::GetCount(lCfg0);

    KMS_TEST_ASSERT(1 <= lKeyboard);
    KMS_TEST_ASSERT(1 <= lMouse);
    KMS_TEST_ASSERT(1 <= lNet);
}

KMS_TEST(Dev_Device2_Config_DI2_W, "Auto", sTest_Config_DI2_W)
{
    char lBuffer[4096];

    Dev::Device2_Config lCfg0;
    Dev::Device2_Config lCfg1;

    lCfg0.mInterface = INVALID_INTERFACE;

    DI2::Code_ASCII_String(&lCfg0, &Dev::Device2_Config_TYPE, sizeof(lBuffer), lBuffer);

    DI2::Decode_ASCII_String(&lCfg1, &Dev::Device2_Config_TYPE, lBuffer);

    KMS_TEST_ASSERT(0 == memcmp(&lCfg0, &lCfg1, sizeof(lCfg0)));
}

KMS_TEST(Dev_Device2_Exception_W, "Auto", sTest_Exception_W)
{
    Dev::Device2_Config lCfg0;

    lCfg0.mInterface = INVALID_INTERFACE;

    Dev::Device2 lD0;

    lD0.Config_Set(lCfg0);

    try
    {
        lD0.Connect();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_NO_DEVICE);
}
