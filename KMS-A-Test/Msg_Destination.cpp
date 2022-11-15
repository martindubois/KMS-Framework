
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Msg_Destination.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>

#include <KMS/Msg/Destination.h>

class TestReceiver : public KMS::Msg::IReceiver
{
public:
    // ===== KMS::Msg::IReceiver ============================================
    unsigned int Receive(void* aSender, unsigned int aCode, void* aData);
};

KMS_TEST(Msg_Destination_Base, "Msg_Destination_Base", "Auto", sTest_Base)
{
    TestReceiver lR;
    KMS::Msg::Destination lD;

    KMS_TEST_ASSERT(!lD.IsSet());

    KMS_TEST_COMPARE(KMS::Msg::IReceiver::MSG_IGNORED, lD.Send(NULL));

    lD.Set(&lR, 1);

    KMS_TEST_ASSERT(lD.IsSet());

    KMS_TEST_COMPARE(0L, lD.Send(NULL));

    lD.Set(&lR, 2);

    KMS_TEST_ASSERT(lD.IsSet());

    KMS_TEST_COMPARE(0L, lD.Send(NULL));

    KMS_TEST_ASSERT(!lD.IsSet());
}

KMS_TEST(Msg_Destination_Exceptions, "Msg_DESTINATION_Exceptions", "Auto", sTest_Exceptions)
{
    TestReceiver lR;
    KMS::Msg::Destination lD(&lR, 3);

    KMS_TEST_COMPARE(KMS::Msg::IReceiver::MSG_EXCEPTION, lD.Send(NULL));

    lD.Set(&lR, 4);

    KMS_TEST_COMPARE(KMS::Msg::IReceiver::MSG_EXCEPTION_STD, lD.Send(NULL));

    lD.Set(&lR, 5);

    KMS_TEST_COMPARE(KMS::Msg::IReceiver::MSG_EXCEPTION_UNKNOWN, lD.Send(NULL));
}

unsigned int TestReceiver::Receive(void* aSender, unsigned int aCode, void* aData)
{
    assert(NULL == aSender);
    assert(NULL == aData);

    unsigned int lResult = 0;

    switch (aCode)
    {
    case 1: break;
    case 2: lResult = KMS::Msg::IReceiver::MSG_ACTION_CLEAR; break;

    case 3:
        KMS::Dbg::gLog.SetHideCount(KMS::Dbg::LogFile::Level::LEVEL_ERROR, 4);
        KMS_EXCEPTION(TEST, "Test exception", "");

    case 4:
        KMS::Dbg::gLog.SetHideCount(KMS::Dbg::LogFile::Level::LEVEL_ERROR, 2);
        throw std::exception();

    case 5:
        KMS::Dbg::gLog.SetHideCount(KMS::Dbg::LogFile::Level::LEVEL_ERROR, 1);
        throw "Test unknown exception";

    default: assert(false);
    }

    return lResult;
}
