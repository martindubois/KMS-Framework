
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
#include <KMS/Process/Browser.h>
#include <KMS/Test/MessageReceiver.h>

#include <KMS/HTTP/ReactApp.h>

// ===== KMS-B-Test =========================================================
#include "../Common/Version.h"

class TestApp : public KMS::Message::IReceiver
{

public:

    TestApp();

    bool GetResult() const;

    // ===== KMS::Message::IReceiver ========================================
    virtual bool Receive(void* aSender, unsigned int aCode, void* aData);

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
    KMS::Process::Browser lB;
    KMS::HTTP::ReactApp lRA;
    TestApp lTA;

    lRA.mServer.mSocket.mAllow.Add("127.0.0.1");

    lRA.AddRoute("/");
    lRA.AddRoute("/Test0");

    lRA.AddFunction("/GetVersion", &lTA, MSG_GET_VERSION);
    lRA.AddFunction("/RunTest0", &lTA, MSG_RUN_TEST_0);

    lRA.mServer.mThread.Start();

    lB.SetPrefered(KMS::Process::Browser::Type::EDGE);

    lB.Open(lRA.mServer, "Test0", "KMS-Framework");

    Sleep(1000);

    lB.Close();

    lRA.mServer.mThread.StopAndWait(1000);

    KMS_TEST_ASSERT(lTA.GetResult());
}

// Public
// //////////////////////////////////////////////////////////////////////////

TestApp::TestApp() : mGetVersion(0), mRunTest0(0)
{
}

bool TestApp::GetResult() const
{
    return (2 == mGetVersion) && (2 == mRunTest0);
}

// ===== KMS::Message::Receiver =============================================

bool TestApp::Receive(void* aSender, unsigned int aCode, void* aData)
{
    KMS::HTTP::Request* lRequest = reinterpret_cast<KMS::HTTP::Request*>(aData);

    bool lResult = false;

    switch (aCode)
    {
    case MSG_GET_VERSION:
        mGetVersion++;

        char lVersion[16];

        VERSION.GetString(lVersion, sizeof(lVersion));

        lRequest->mResponseHeader.Set("Access-Control-Allow-Origin", "*");
        lRequest->mResponseData.Set("Version", lVersion);
        lResult = true;
        break;

    case MSG_RUN_TEST_0:
        mRunTest0++;

        lRequest->mResponseHeader.Set("Access-Control-Allow-Origin", "*");
        lResult = true;
        break;

    default: assert(false);
    }

    return lResult;
}
