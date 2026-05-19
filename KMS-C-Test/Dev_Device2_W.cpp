
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

using namespace KMS;

// {586DBACF-6F76-4448-85C4-DEF8DE99C438}
static const GUID INVALID_INTERFACE = { 0x586dbacf, 0x6f76, 0x4448, { 0x85, 0xc4, 0xde, 0xf8, 0xde, 0x99, 0xc4, 0x38 } };

KMS_TEST(Dev_Device2_Windows_Config, "Auto", sTest_Windows)
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

KMS_TEST(Dev_Device2_Windows_Exception, "Auto", sTest_Windows_Exception)
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
