
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/WOP_Receiver.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/BitArray.h>
#include <KMS/WOP/Receiver.h>
#include <KMS/WOP/ValueArray.h>

KMS_TEST(WOP_Receiver_Base, "WOP_Receiver_Base", "Auto", sTest_Base)
{
    static const uint8_t FRAME_A[] = { 0x00, 0xff, 0x01, 0x01, 0x0a, 0xc4, 0x43 };
    static const uint8_t FRAME_B[] = { 0x00, 0x00, 0x01, 0x01, 0x00, 0x74, 0x50 };
    static const uint8_t FRAME_C[] = { 0x00, 0x00, 0x00, 0x01, 0x02, 0xa4, 0x51 };
    static const uint8_t FRAME_D[] = { 0x00, 0xff, 0x00, 0x01, 0x02, 0x94, 0x45 };
    static const uint8_t FRAME_E[] = { 0x00, 0xff, 0x00, 0x01, 0x02, 0x94, 0x46 }; // Bad CRC

    KMS::WOP::BitArray<uint8_t>       lBA0;
    KMS::WOP::Object                * lInstances[2];
    KMS::WOP::System                  lSys0;
    KMS::WOP::ValueArray<uint16_t, 2> lVA0;

    lInstances[0] = &lBA0;
    lInstances[1] = &lVA0;

    // Constructor
    KMS::WOP::Receiver lR0(&lSys0, lInstances, 2);

    // AddReceivedByte
    lR0.AddReceivedByte(0xff);

    // AddReceivedBytes
    lR0.AddReceivedBytes(FRAME_A, sizeof(FRAME_A));
    KMS_TEST_ASSERT(KMS_WOP_RESULT_TEST == lSys0.GetResult());

    lR0.AddReceivedBytes(FRAME_B, sizeof(FRAME_B));
    KMS_TEST_ASSERT(0x00 == lBA0.GetBits());

    lR0.AddReceivedBytes(FRAME_C, sizeof(FRAME_C));
    lR0.AddReceivedBytes(FRAME_D, sizeof(FRAME_D));
    lR0.AddReceivedBytes(FRAME_E, sizeof(FRAME_E));

    // SetInstanceCount
    lR0.SetInstanceCount(2);
}
