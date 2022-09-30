
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/GUI_DynamicForm.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/NetAddressRange.h>
#include <KMS/DI/String.h>
#include <KMS/GUI/Form.h>
#include <KMS/GUI/MetaData.h>
#include <KMS/HTTP/ReactApp.h>
#include <KMS/HTTP/Request.h>
#include <KMS/Proc/Browser.h>

// ===== KMS-B-Test =========================================================
#include "../Common/Version.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_VERSION_GET_DATA (1)

static const KMS::GUI::MetaData MD_RESULT("", "Result");

class TestApp1 : public KMS::Msg::IReceiver
{

public:

    TestApp1();

    bool GetResult() const;

    // ===== KMS::Msg::IReceiver ============================================
    virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

    KMS::GUI::Form mForm;

private:

    KMS::DI::Dictionary mDictionary;

    KMS::DI::String mResult;

};

KMS_TEST(GUI_DynamicForm_Interactive, "GUI_DynamicForm_Interactive", "Interactive", sTest_Interactive)
{
    KMS::Proc::Browser    lB;
    KMS::HTTP::ReactApp   lRA;

    /* TODO lD.SetEntry("Result", new KMS::JSON::String("Pending")); */

    TestApp1 lTA;

    /* lTA.mForm.AddField("Result", "Result");

    lTA.mForm.SetObject(&lD); */

    lRA.mServer.mSocket.mAllowedRanges.AddEntry(new KMS::DI::NetAddressRange("127.0.0.1"), true);

    lRA.AddRoute("/");
    lRA.AddRoute("/TestDynamicForm");

    lRA.AddFunction("/Version/GetData", &lTA, MSG_VERSION_GET_DATA);

    lTA.mForm.Connect(&lRA, "DynamicForm");

    lRA.mServer.mThread.Start();

    lB.SetPrefered(KMS::Proc::Browser::Type::EDGE);

    lB.Open(lRA.mServer, "TestDynamicForm", "KMS-Framework");

    lB.Wait(600000); // ms = 600 s = 10 min

    lRA.mServer.mThread.StopAndWait(2000); // ms

    KMS_TEST_ASSERT(lTA.GetResult());
}

// Public
// //////////////////////////////////////////////////////////////////////////

TestApp1::TestApp1() : mResult("Pending")
{
    mDictionary.AddEntry("Result", &mResult, false, &MD_RESULT);

    mForm.SetDictionary(&mDictionary);
}

bool TestApp1::GetResult() const { return "PASSED" == mResult; }

// ===== KMS::Msg::Receiver =================================================

unsigned int TestApp1::Receive(void* aSender, unsigned int aCode, void* aData)
{
    KMS::HTTP::Request* lRequest = reinterpret_cast<KMS::HTTP::Request*>(aData);

    unsigned int lResult = 0;

    switch (aCode)
    {
    case MSG_VERSION_GET_DATA:
        char lVersion[16];

        VERSION.GetString(lVersion, sizeof(lVersion));

        lRequest->mResponseHeader.AddEntry("Access-Control-Allow-Origin", new KMS::DI::String("*"), true);

        lRequest->mResponseData.AddEntry("Version", new KMS::DI::String(lVersion), true);
        break;

    default:
        assert(false);
        lResult = KMS::Msg::IReceiver::MSG_IGNORED;
    }

    return lResult;
}
