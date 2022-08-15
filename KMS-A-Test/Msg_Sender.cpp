
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Msg_Sender.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>

#include <KMS/Msg/Sender.h>

class TestReceiver : public KMS::Msg::IReceiver
{
public:
    // ===== KMS::Msg::IReceiver ============================================
    unsigned int Receive(void* aSender, unsigned int aCode, void* aData);
};

KMS_TEST(Msg_Sender_Base, "Msg_Sender_Base", "Auto", sTest_Base)
{
    TestReceiver lR;
    KMS::Msg::Sender lS(NULL);

    KMS_TEST_ASSERT(!lS.IsSet());

    KMS_TEST_COMPARE(KMS::Msg::IReceiver::MSG_IGNORED, lS.Send());

    lS.Set(&lR, 1);

    KMS_TEST_ASSERT(lS.IsSet());

    KMS_TEST_COMPARE(0L, lS.Send());

    lS.Set(&lR, 2);

    KMS_TEST_ASSERT(lS.IsSet());

    KMS_TEST_COMPARE(0L, lS.Send());

    KMS_TEST_ASSERT(!lS.IsSet());
}

KMS_TEST(Msg_Sender_Exceptions, "Msg_Sender_Exceptions", "Auto", sTest_Exceptions)
{
    TestReceiver lR;
    KMS::Msg::Sender lS(NULL, &lR, 3);

    KMS_TEST_COMPARE(KMS::Msg::IReceiver::MSG_EXCEPTION, lS.Send());

    lS.Set(&lR, 4);

    KMS_TEST_COMPARE(KMS::Msg::IReceiver::MSG_EXCEPTION_STD, lS.Send());

    lS.Set(&lR, 5);

    KMS_TEST_COMPARE(KMS::Msg::IReceiver::MSG_EXCEPTION_UNKNOWN, lS.Send());
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
    case 3: KMS_EXCEPTION(TEST, "Test exception");
    case 4: throw std::exception();
    case 5: throw "Test unknown exception";

    default: assert(false);
    }

    return lResult;
}
