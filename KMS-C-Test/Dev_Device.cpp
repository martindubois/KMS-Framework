
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C-Test/Dev_Device.cpp

#include "Component.h"

// ===== Windows ============================================================
#define INITGUID
#include <Windows.h>
#include <Ndisguid.h>
#include <Ntddmou.h>
#include <Ntddkbd.h>

// ===== Includes ===========================================================
#include <KMS/Dev/Device.h>

using namespace KMS;

// {586DBACF-6F76-4448-85C4-DEF8DE99C438}
static const GUID INVALID_INTERFACE = { 0x586dbacf, 0x6f76, 0x4448, { 0x85, 0xc4, 0xde, 0xf8, 0xde, 0x99, 0xc4, 0x38 } };

KMS_TEST(Dev_Device_Windows, "Auto", sTest_Windows)
{
    KMS_TEST_COMPARE(Dev::Device::GetCount(&INVALID_INTERFACE), 0L);

    auto lComPort  = Dev::Device::GetCount(&GUID_DEVINTERFACE_COMPORT);
    auto lKeyboard = Dev::Device::GetCount(&GUID_DEVINTERFACE_KEYBOARD);
    auto lMouse    = Dev::Device::GetCount(&GUID_DEVINTERFACE_MOUSE);
    auto lNet      = Dev::Device::GetCount(&GUID_DEVINTERFACE_NET);

    KMS_TEST_ASSERT(0 < lKeyboard);
    KMS_TEST_ASSERT(0 < lMouse);
    KMS_TEST_ASSERT(0 < lNet);
}
