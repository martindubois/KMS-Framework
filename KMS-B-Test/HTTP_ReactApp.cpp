
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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

};

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_GET_VERSION (1)

KMS_TEST(HTTP_ReactApp_Base, "HTTP_ReactApp_Base", "Auto", sTest_Base)
{
    KMS::Proc::Browser lB;
    KMS::HTTP::ReactApp lRA;
    TestApp lTA;

    lRA.mServer.mSocket.mAllowedRanges.AddEntry(new KMS::DI::NetAddressRange("127.0.0.1", &KMS::DI::META_DATA_DELETE_OBJECT));

    lRA.AddRoute("/");

    lRA.AddFunction("/Version/GetData", &lTA, MSG_GET_VERSION);

    lRA.mServer.mThread.Start();

    lB.SetPrefered(KMS::Proc::Browser::Type::EDGE);

    lB.Open(lRA.mServer, "", "KMS-Framework");

    Sleep(2000);

    lB.Close();

    lRA.mServer.mThread.StopAndWait(2000);

    KMS_TEST_ASSERT(lTA.GetResult());
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

// ===== KMS::Msg::Receiver =================================================

static const KMS::DI::MetaData MD_ACCESS_CONTROL_ALLOW_ORIGIN("Access-Control-Allow-Origin", NULL);
static const KMS::DI::MetaData MD_VERSION                    ("Version", NULL, KMS::DI::MetaData::FLAG_DELETE_OBJECT);

static const KMS::DI::String ACCESS_CONTROL_ALLOW_ORIGIN("*", &MD_ACCESS_CONTROL_ALLOW_ORIGIN);

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

        lRequest->mResponseHeader.AddEntry(&ACCESS_CONTROL_ALLOW_ORIGIN);

        lRequest->mResponseData.AddEntry(new KMS::DI::String(lVersion, &MD_VERSION));
        break;

    default:
        assert(false);
        lResult = KMS::Msg::IReceiver::MSG_IGNORED;
    }

    return lResult;
}
