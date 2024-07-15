
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C-Test/Dev_IDevice.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dev/Device.h>

using namespace KMS;

KMS_TEST(Dev_IDevice_Base, "Auto", sTest_Base)
{
    uint8_t lBuffer[2048];

    Dev::Device lD0;
    Dev::Device lD1;

    lD0.mLink = "KMS-C-Test/Tests/Test_Dev_IDevice_Base_0.txt";
    lD1.mLink = "KMS-C-Test/Tests/Test_Dev_IDevice_Base_0.txt";

    Dev::IDevice* lID0 = &lD0;
    Dev::IDevice* lID1 = &lD1;

    // ===== Dev::IDevice ===================================================

    // SetConnectFlags
    lID0->SetConnectFlags(Dev::IDevice::FLAG_ACCESS_READ);

    // ClearReadBuffer

    // Control

    // ===== Stream::IStream ================================================

    // IsConnected
    KMS_TEST_ASSERT(!lID0->IsConnected());

    // Connect
    lID0->Connect();
    lID0->Connect();

    // Disconnect
    lID0->Disconnect();

    // Read
    lID0->Connect();
    KMS_TEST_COMPARE(lID0->Read(lBuffer, sizeof(lBuffer)), 211U);
    lID0->Disconnect();

    // Write
    lID1->SetConnectFlags(Dev::IDevice::FLAG_ACCESS_WRITE);
    lID1->Connect();
    KMS_TEST_ASSERT(lID1->Write(lBuffer, 1));

    // ===== Dev::Device ====================================================

    // operator HANDLE
    KMS_TEST_ASSERT(INVALID_HANDLE_VALUE != lD1);

    // ----- DI::Container --------------------------------------------------

    // Validate
    lD0.Validate();
}

KMS_TEST(Dev_IDevice_Exception, "Auto", sTest_Exception)
{
    uint8_t lBuffer[256];

    Dev::Device lD0;

    lD0.mLink = "DoesNotExist";

    Dev::IDevice* lID0 = &lD0;

    // ===== Stream::IStream ================================================

    // Connect
    lD0.SetConnectFlags(Dev::IDevice::FLAG_ACCESS_READ);
    try
    {
        lD0.Connect();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_CONNECT_FAILED);

    // Control
    lD0.mLink = "KMS-C-Test/Tests/Test_Dev_IDevice_Base_0.txt";
    lID0->Connect();
    try
    {
        lD0.Control(0, nullptr, 0, nullptr, 0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_CONTROL_FAILED");

    // Read
    lD0.SetConnectFlags(Dev::IDevice::FLAG_ACCESS_WRITE);
    lID0->Connect();
    try
    {
        lD0.Read(lBuffer, sizeof(lBuffer));
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_READ_FAILED);

    lD0.SetConnectFlags(Dev::IDevice::FLAG_ACCESS_READ);
    lID0->Connect();
    try
    {
        lD0.Read(lBuffer, sizeof(lBuffer), Dev::IDevice::FLAG_READ_ALL);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_READ_FAILED);
}
