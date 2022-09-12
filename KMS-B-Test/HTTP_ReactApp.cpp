
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/HTTP_ReactApp.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/HTTP/Request.h>
#include <KMS/JSON/String.h>
#include <KMS/JSON/Value.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/Proc/Browser.h>

#include <KMS/HTTP/ReactApp.h>

// ===== KMS-B-Test =========================================================
#include "../Common/Version.h"

class TestApp : public KMS::Msg::IReceiver
{

public:

    TestApp();

    bool GetResult() const;

    // ===== KMS::Msg::IReceiver ============================================
    virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

private:

    unsigned int mGetVersion;
    unsigned int mRunTest0;

};

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_GET_VERSION (1)
#define MSG_RUN_TEST_0  (2)

KMS_TEST(HTTP_ReactApp_Base, "HTTP_ReactApp_Base", "Auto", sTest_Base)
{
    KMS::Proc::Browser lB;
    KMS::HTTP::ReactApp lRA;
    TestApp lTA;

    lRA.mServer.mSocket.mAllow.Add("127.0.0.1");

    lRA.AddRoute("/");
    lRA.AddRoute("/Test0");

    lRA.AddFunction("/GetVersion", &lTA, MSG_GET_VERSION);
    lRA.AddFunction("/RunTest0", &lTA, MSG_RUN_TEST_0);

    lRA.mServer.mThread.Start();

    lB.SetPrefered(KMS::Proc::Browser::Type::EDGE);

    lB.Open(lRA.mServer, "Test0", "KMS-Framework");

    Sleep(1000);

    lB.Close();

    lRA.mServer.mThread.StopAndWait(2000);

    KMS_TEST_ASSERT(lTA.GetResult());
}

// Public
// //////////////////////////////////////////////////////////////////////////

TestApp::TestApp() : mGetVersion(0), mRunTest0(0)
{
}

bool TestApp::GetResult() const
{
    return (1 <= mGetVersion) && (1 <= mRunTest0);
}

// ===== KMS::Msg::Receiver =================================================

unsigned int TestApp::Receive(void* aSender, unsigned int aCode, void* aData)
{
    KMS::HTTP::Request* lRequest = reinterpret_cast<KMS::HTTP::Request*>(aData);

    unsigned int lResult = 0;

    switch (aCode)
    {
    case MSG_GET_VERSION:
        mGetVersion++;

        char lVersion[16];

        VERSION.GetString(lVersion, sizeof(lVersion));

        lRequest->mResponseHeader.SetEntry("Access-Control-Allow-Origin", new KMS::JSON::String("*"));

        lRequest->mResponseData.SetEntry("Version", new KMS::JSON::String(lVersion));
        break;

    case MSG_RUN_TEST_0:
        mRunTest0++;

        lRequest->mResponseHeader.SetEntry("Access-Control-Allow-Origin", new KMS::JSON::String("*"));
        break;

    default:
        assert(false);
        lResult = KMS::Msg::IReceiver::MSG_IGNORED;
    }

    return lResult;
}
