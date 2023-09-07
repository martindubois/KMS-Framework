
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Msg_Destination.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>

#include <KMS/Msg/Destination.h>

using namespace KMS;

class TestReceiver : public Msg::IReceiver
{
public:
    // ===== Msg::IReceiver =================================================
    unsigned int Receive(void* aSender, unsigned int aCode, void* aData);
};

KMS_TEST(Msg_Destination_Base, "Auto", sTest_Base)
{
    TestReceiver lR;
    Msg::Destination lD;

    KMS_TEST_ASSERT(!lD.IsSet());

    KMS_TEST_COMPARE(Msg::IReceiver::MSG_IGNORED, lD.Send(nullptr));

    lD.Set(&lR, 1);

    KMS_TEST_ASSERT(lD.IsSet());

    KMS_TEST_COMPARE(0L, lD.Send(nullptr));

    lD.Set(&lR, 2);

    KMS_TEST_ASSERT(lD.IsSet());

    KMS_TEST_COMPARE(0L, lD.Send(nullptr));

    KMS_TEST_ASSERT(!lD.IsSet());
}

KMS_TEST(Msg_Destination_Exception, "Auto", sTest_Exception)
{
    TestReceiver lR;
    Msg::Destination lD(&lR, 3);

    KMS_TEST_COMPARE(Msg::IReceiver::MSG_EXCEPTION, lD.Send(nullptr));

    lD.Set(&lR, 4);

    KMS_TEST_COMPARE(Msg::IReceiver::MSG_EXCEPTION_STD, lD.Send(nullptr));

    lD.Set(&lR, 5);

    KMS_TEST_COMPARE(Msg::IReceiver::MSG_EXCEPTION_UNKNOWN, lD.Send(nullptr));
}

unsigned int TestReceiver::Receive(void* aSender, unsigned int aCode, void* aData)
{
    assert(nullptr == aSender);
    assert(nullptr == aData);

    unsigned int lResult = 0;

    switch (aCode)
    {
    case 1: break;
    case 2: lResult = Msg::IReceiver::MSG_ACTION_CLEAR; break;

    case 3:
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 4);
        KMS_EXCEPTION(RESULT_ERROR, "Test exception", "");

    case 4:
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        throw std::exception();

    case 5:
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        throw "Test unknown exception";

    default: assert(false);
    }

    return lResult;
}
