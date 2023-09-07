
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/WOP_System.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/BitArray.h>
#include <KMS/WOP/System.h>
#include <KMS/WOP/ValueArray.h>

using namespace KMS;

// ===== Local ==============================================================
#include "../Common/Version.h"

KMS_TEST(WOP_System_Base, "Auto", sTest_Base)
{
    static const uint8_t FRAME_E[] = { 0x00, 0xff, 0x00, 0x01, 0x02, 0x94, 0x46 }; // Bad CRC

    WOP::BitArray<uint8_t>       lBA0;
    WOP::Object                * lInstances[2];
    WOP::ValueArray<uint16_t, 2> lVA0;

    lInstances[0] = &lBA0;
    lInstances[1] = &lVA0;

    // Constructor
    WOP::System lS0(VERSION, 'tseT', 0x00);

    lS0.mConsole.Set_Null();

    // GetResult
    KMS_TEST_ASSERT(0 == lS0.GetResult());

    // GetVersion
    const Version& lV0 = lS0.GetVersion();

    // SetInstances
    lS0.SetInstances(lInstances, 2);

    // SetResult
    lS0.SetResult(KMS_WOP_RESULT_OK);

    // AddReceivedByte
    lS0.AddReceivedByte(0xff);

    // AddReceivedBytes / PrepareFrame

    // Frame #1 - Write System::mResult_In (KMS_WOP_RESULT_INVALID_CONTROL_VALUE)
    auto lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(nullptr != lF0);
    lS0.AddReceivedBytes(reinterpret_cast<const uint8_t*>(lF0->GetRawFrame()), lF0->GetFrameSize_byte());
    KMS_TEST_ASSERT(KMS_WOP_RESULT_INVALID_CONTROL_VALUE == lS0.GetResult());
    KMS_TEST_ASSERT(KMS_WOP_RESULT_OK == lS0.GetResult());
    KMS_TEST_ASSERT(nullptr == lS0.PrepareFrame());

    // Frame #2 - Bad CRC
    lS0.AddReceivedBytes(FRAME_E, sizeof(FRAME_E));

    // Frame #3 - Write System::mResult_In (KMS_WOP_RESULT_INVALID_CRC)
    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(nullptr != lF0);
    lS0.AddReceivedBytes(reinterpret_cast<const uint8_t*>(lF0->GetRawFrame()), lF0->GetFrameSize_byte());
    KMS_TEST_ASSERT(KMS_WOP_RESULT_INVALID_CRC == lS0.GetResult());
    KMS_TEST_ASSERT(nullptr == lS0.PrepareFrame());

    // Frame #4 - Write lBA0
    lBA0.AddRequest(0x02);
    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(nullptr != lF0);
    lS0.AddReceivedBytes(reinterpret_cast<const uint8_t*>(lF0->GetRawFrame()), lF0->GetFrameSize_byte());
    KMS_TEST_ASSERT(nullptr == lS0.PrepareFrame());

    // Frame #5 - Write System::mRequest_In (0x01)
    lS0.SendRequest(0x01);
    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(nullptr != lF0);
    lS0.AddReceivedBytes(reinterpret_cast<const uint8_t*>(lF0->GetRawFrame()), lF0->GetFrameSize_byte());

    // Frame #6 - Write System::mRequest_In (0x01)
    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(nullptr != lF0);
    KMS_TEST_ASSERT(nullptr == lS0.PrepareFrame());

    // Frame #7 - Write System::mProtocol_In
    lS0.AddRequest(0x02);
    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(nullptr != lF0);
    lS0.AddReceivedBytes(reinterpret_cast<const uint8_t*>(lF0->GetRawFrame()), lF0->GetFrameSize_byte());
    KMS_TEST_ASSERT(nullptr == lS0.PrepareFrame());

    // Frame #8 - Write System::mVersion_In
    lS0.AddRequest(0x10);
    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(nullptr != lF0);
    lS0.AddReceivedBytes(reinterpret_cast<const uint8_t*>(lF0->GetRawFrame()), lF0->GetFrameSize_byte());
    KMS_TEST_ASSERT(nullptr == lS0.PrepareFrame());

    // Frame #9 - Write mTrace_In
    lS0.AddTrace("Trace\r\n", 7);
    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(nullptr != lF0);
    lS0.AddReceivedBytes(reinterpret_cast<const uint8_t*>(lF0->GetRawFrame()), lF0->GetFrameSize_byte());
    KMS_TEST_ASSERT(nullptr == lS0.PrepareFrame());

    // AddTrace
    lS0.AddTrace("Very long trace\r\n", 17);
    lS0.AddTrace("Very long trace\r\n", 17);

    // Dump
    lS0.Dump();
}
