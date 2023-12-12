
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/GUI_DynamicForm.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/NetAddressRange.h>
#include <KMS/DI/String.h>
#include <KMS/GUI/Form.h>
#include <KMS/GUI/MetaData.h>
#include <KMS/HTTP/HTTP.h>
#include <KMS/HTTP/ReactApp.h>
#include <KMS/HTTP/Transaction.h>
#include <KMS/Proc/Browser.h>

using namespace KMS;

// ===== KMS-B-Test =========================================================
#include "../Common/Version.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

static const GUI::MetaData MD_RESULT("", "Result");

class TestApp1
{

public:

    const Callback<TestApp1> ON_VERSION_GET_DATA;

    TestApp1();

    bool GetResult() const;

    GUI::Form mForm;

private:

    // ===== Callbacks ======================================================
    unsigned int OnVersionGetData(void* aSender, void* aData);

    DI::Dictionary mDictionary;

    DI::String mResult;

};

KMS_TEST(GUI_DynamicForm_Interactive, "Interactive", sTest_Interactive)
{
    Proc::Browser    lB;
    HTTP::ReactApp   lRA;

    /* TODO lD.SetEntry("Result", new JSON::String("Pending")); */

    TestApp1 lTA;

    /* lTA.mForm.AddField("Result", "Result");

    lTA.mForm.SetObject(&lD); */

    lRA.mServer.mSocket.mAllowedRanges.AddEntry(new DI::NetAddressRange("127.0.0.1"), true);

    lRA.AddRoute("/");
    lRA.AddRoute("/TestDynamicForm");

    lRA.AddFunction("/Version/GetData", &lTA.ON_VERSION_GET_DATA);

    lTA.mForm.Connect(&lRA, "DynamicForm");

    lRA.mServer.mThread.Start();

    lB.SetPrefered(Proc::Browser::Type::EDGE);

    lB.Open(lRA.mServer, "TestDynamicForm", "KMS-Framework");

    lB.Wait(600000); // ms = 600 s = 10 min

    lRA.mServer.mThread.StopAndWait(2000); // ms

    KMS_TEST_ASSERT(lTA.GetResult());
}

// Public
// //////////////////////////////////////////////////////////////////////////

TestApp1::TestApp1()
    : mResult("Pending")
    // ===== Callbacks ======================================================
    , ON_VERSION_GET_DATA(this, &TestApp1::OnVersionGetData)
{
    mDictionary.AddEntry("Result", &mResult, false, &MD_RESULT);

    mForm.SetDictionary(&mDictionary);
}

bool TestApp1::GetResult() const { return "PASSED" == mResult; }

// Private
// //////////////////////////////////////////////////////////////////////////

unsigned int TestApp1::OnVersionGetData(void*, void* aData)
{
    assert(nullptr != aData);

    HTTP::Transaction* lTransaction = reinterpret_cast<HTTP::Transaction*>(aData);

    char lVersion[16];

    VERSION.GetString(lVersion, sizeof(lVersion));

    lTransaction->mResponse_Header.AddConstEntry(HTTP::Response::FIELD_NAME_ACCESS_CONTROL_ALLOW_ORIGIN, &HTTP::Response::FIELD_VALUE_ACCESS_CONTROL_ALLOW_ORIGIN_ALL);

    lTransaction->mResponse_Data.AddEntry("Version", new DI::String(lVersion), true);

    return 0;
}
