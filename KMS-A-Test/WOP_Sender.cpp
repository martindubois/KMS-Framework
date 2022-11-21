
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/WOP_Sender.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/BitArray.h>
#include <KMS/WOP/Sender.h>
#include <KMS/WOP/ValueArray.h>

using namespace KMS;

// ===== Local ==============================================================
#include "../Common/Version.h"

KMS_TEST(WOP_Sender_Base, "WOP_Sender_Base", "Auto", sTest_Base)
{
    WOP::BitArray<uint8_t>       lBA0;
    WOP::Object                * lInstances[2];
    WOP::System                  lSys0(VERSION);
    WOP::ValueArray<uint16_t, 2> lVA0;

    lInstances[0] = &lBA0;
    lInstances[1] = &lVA0;

    // Constructor
    WOP::Sender lS0(&lSys0, lInstances, 2);

    // SetInstanceCount
    lS0.SetInstanceCount(2);

    // PrepareFrame
    KMS_TEST_ASSERT(NULL == lS0.PrepareFrame());

    lSys0.SetResult(KMS_WOP_RESULT_TEST);

    const WOP::FrameBuffer* lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(NULL != lF0);

    KMS_TEST_ASSERT(NULL == lS0.PrepareFrame());

    lSys0.SendRequest(0x02);

    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(NULL != lF0);

    KMS_TEST_ASSERT(NULL == lS0.PrepareFrame());

    lBA0.AddRequest(0x02);

    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(NULL != lF0);

    KMS_TEST_ASSERT(NULL == lS0.PrepareFrame());

    lBA0.SendRequest(0x02);

    lF0 = lS0.PrepareFrame();
    KMS_TEST_ASSERT(NULL != lF0);

    KMS_TEST_ASSERT(NULL == lS0.PrepareFrame());

    lBA0.AddRequest(0x02);
}
