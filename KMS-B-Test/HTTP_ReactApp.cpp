
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/HTTP_ReactApp.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <thread>

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/NetAddressRange.h>
#include <KMS/DI/String.h>
#include <KMS/HTTP/HTTP.h>
#include <KMS/HTTP/Transaction.h>
#include <KMS/Proc/Browser.h>

#include <KMS/HTTP/ReactApp.h>

using namespace KMS;

// ===== KMS-B-Test =========================================================
#include "../Common/Version.h"

class TestApp
{

public:

    const Callback<TestApp> ON_GET_VERSION;

    TestApp();

    bool GetResult() const;

private:

    // ===== Callbacks ======================================================
    unsigned int OnGetVersion(void* aSender, void* aData);

    unsigned int mGetVersion;

};

// Constants
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(HTTP_ReactApp_Base, "Auto", sTest_Base)
{
    Proc::Browser lB;
    HTTP::ReactApp lRA;
    TestApp lTA;

    Ptr_OF<DI::Object> lEntry(new DI::NetAddressRange("127.0.0.1"), true);
    lRA.mServer.mSocket.mAllowedRanges.AddEntry(lEntry);

    lRA.AddFunction("/Version/GetData", &lTA.ON_GET_VERSION);

    lRA.mServer.Start();

    lB.SetPrefered(Proc::Browser::Type::EDGE);

    KMS_TEST_OUTPUT_BEGIN();
    {
        lB.Open(lRA.mServer, "", "KMS-Framework");

        for (unsigned int i = 0; i < 10; i++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (lTA.GetResult())
            {
                break;
            }

            std::cout << "1 second" << std::endl;
        }

        KMS_TEST_ASSERT(lTA.GetResult());

        lB.Close();

        lRA.mServer.StopAndWait(2000);
    }
    KMS_TEST_OUTPUT_END();
}

// Public
// //////////////////////////////////////////////////////////////////////////

TestApp::TestApp()
    : mGetVersion(0)
    // ===== Callbacks ======================================================
    , ON_GET_VERSION(this, &TestApp::OnGetVersion)
{}

bool TestApp::GetResult() const
{
    return 1 <= mGetVersion;
}

// Private
// //////////////////////////////////////////////////////////////////////////

#define NAME_VERSION                     "Version"

unsigned int TestApp::OnGetVersion(void* aSender, void* aData)
{
    assert(nullptr != aData);

    auto lTransaction = reinterpret_cast<HTTP::Transaction*>(aData);

    std::cout << "MSG_GET_VERSION" << std::endl;

    mGetVersion++;

    char lVersion[16];

    VERSION.GetString(lVersion, sizeof(lVersion));

    Ptr_OF<DI::Object> lEntry;

    lEntry.Set(&HTTP::Response::FIELD_VALUE_ACCESS_CONTROL_ALLOW_ORIGIN_ALL);

    lTransaction->mResponse_Header.AddEntry(HTTP::Response::FIELD_NAME_ACCESS_CONTROL_ALLOW_ORIGIN, lEntry);

    auto lData = new DI::Dictionary;

    lEntry.Set(new DI::String(lVersion), true);

    lData->AddEntry(NAME_VERSION, lEntry);

    Ptr_OF<DI::Object> lResponseData(lData, true);

    lTransaction->SetResponseData(lResponseData);

    return 0;
}
