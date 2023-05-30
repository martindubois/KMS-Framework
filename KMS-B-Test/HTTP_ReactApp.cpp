
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/HTTP_ReactApp.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/NetAddressRange.h>
#include <KMS/DI/String.h>
#include <KMS/HTTP/Request.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/Proc/Browser.h>

#include <KMS/HTTP/ReactApp.h>

using namespace KMS;

// ===== KMS-B-Test =========================================================
#include "../Common/Version.h"

class TestApp : public Msg::IReceiver
{

public:

    TestApp();

    bool GetResult() const;

    // ===== Msg::IReceiver =================================================
    virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

private:

    unsigned int mGetVersion;

};

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_GET_VERSION (1)

KMS_TEST(HTTP_ReactApp_Base, "HTTP_ReactApp_Base", "Auto", sTest_Base)
{
    Proc::Browser lB;
    HTTP::ReactApp lRA;
    TestApp lTA;

    lRA.mServer.mSocket.mAllowedRanges.AddEntry(new DI::NetAddressRange("127.0.0.1"), true);

    lRA.AddRoute("/");

    lRA.AddFunction("/Version/GetData", &lTA, MSG_GET_VERSION);

    lRA.mServer.mThread.Start();

    lB.SetPrefered(Proc::Browser::Type::EDGE);

    lB.Open(lRA.mServer, "", "KMS-Framework");

    for (unsigned int i = 0; i < 10; i++)
    {
        Sleep(1000);

        if (lTA.GetResult())
        {
            break;
        }

        std::cout << "1 second" << std::endl;
    }

    KMS_TEST_ASSERT(lTA.GetResult());

    lB.Close();

    lRA.mServer.mThread.StopAndWait(2000);
}

// Public
// //////////////////////////////////////////////////////////////////////////

TestApp::TestApp() : mGetVersion(0)
{
}

bool TestApp::GetResult() const
{
    return 1 <= mGetVersion;
}

// ===== Msg::Receiver ======================================================

#define NAME_ACCESS_CONTROL_ALLOW_ORIGIN "Access-Control-Allow-Origin"
#define NAME_VERSION                     "Version"

static const DI::String ACCESS_CONTROL_ALLOW_ORIGIN("*");

unsigned int TestApp::Receive(void* aSender, unsigned int aCode, void* aData)
{
    auto lRequest = reinterpret_cast<HTTP::Request*>(aData);

    unsigned int lResult = 0;

    switch (aCode)
    {
    case MSG_GET_VERSION:
        std::cout << "MSG_GET_VERSION" << std::endl;

        mGetVersion++;

        char lVersion[16];

        VERSION.GetString(lVersion, sizeof(lVersion));

        lRequest->mResponseHeader.AddConstEntry(NAME_ACCESS_CONTROL_ALLOW_ORIGIN, &ACCESS_CONTROL_ALLOW_ORIGIN);

        lRequest->mResponseData.AddEntry(NAME_VERSION, new DI::String(lVersion), true);
        break;

    default:
        assert(false);
        lResult = Msg::IReceiver::MSG_IGNORED;
    }

    return lResult;
}
