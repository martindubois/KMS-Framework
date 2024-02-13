
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/HTTP_Client.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/HTTP/Client.h>

using namespace KMS;

KMS_TEST(HTTP_Client_Base, "Auto", sTest_Base)
{
    File::Binary lOF0(File::Folder::CURRENT, "Test_HTTP_Client_Base_0.html", true);
    File::Binary lOF1(File::Folder::CURRENT, "Test_HTTP_Client_Base_1.html", true);
    File::Binary lOF2(File::Folder::CURRENT, "Test_HTTP_Client_Base_2.zip" , true);

    Net::Thread_Startup();

    // Constructor
    HTTP::Client lC0(false);
    HTTP::Client lC1(true);
    HTTP::Client lC2(true);
    HTTP::Client lC3(false);
    HTTP::Client lC4(false);
    HTTP::Client lC5(false);
    HTTP::Client lC6(false);

    // GetResponseData
    KMS_TEST_ASSERT(nullptr == lC0.GetResponseData());

    // Get
    lC0.Get("http://www.kms-quebec.com/MaPorteDuWEB/index.htm", &lOF0);
    lC1.Get("https://www.kms-quebec.com/MaPorteDuWEB/index.htm", &lOF1);
    lC2.Get("https://github.com/martindubois/KMS-Framework/releases/download/1.0.1_Windows/KMS-Framework_Windows_1.0.1.zip", &lOF2);
    lC3.Get("http://www.kms-quebec.com/back-end/index.php/version/get");
    lC4.Get("http://www.kms-quebec.com/back-end/index.php/card/get?Date=2024-01-10");

    auto lObj = lC3.GetResponseData();
    KMS_TEST_ASSERT(nullptr != lObj);

    auto lDict = dynamic_cast<const DI::Dictionary*>(lObj);
    KMS_TEST_ASSERT(nullptr != lDict);
    KMS_TEST_COMPARE(lDict->GetCount(), 1U);

    lObj = lC4.GetResponseData();
    KMS_TEST_ASSERT(nullptr != lObj);

    auto lArray = dynamic_cast<const DI::Array*>(lObj);
    KMS_TEST_ASSERT(nullptr != lArray);
    KMS_TEST_COMPARE(lArray->GetCount(), 2U);

    // Post
    DI::Dictionary lData;
    lData.AddEntry("Billed"     , new DI::UInt<uint32_t>(0)   , true);
    lData.AddEntry("Client"     , new DI::String("KMS")       , true);
    lData.AddEntry("Date"       , new DI::String("2024-01-12"), true);
    lData.AddEntry("Description", new DI::String("Test")      , true);
    lData.AddEntry("Distance_km", new DI::UInt<uint32_t>(0)   , true);
    lData.AddEntry("Hidden"     , new DI::UInt<uint32_t>(0)   , true);
    // lC5.Post("http://www.kms-quebec.com/back-end/index.php/travel/new", &lData);

    Net::Thread_Cleanup();

    // ===== Cleanup ========================================================
    File::Folder::CURRENT.Delete("Test_HTTP_Client_Base_0.html");
    File::Folder::CURRENT.Delete("Test_HTTP_Client_Base_1.html");
    File::Folder::CURRENT.Delete("Test_HTTP_Client_Base_2.zip");
}

KMS_TEST(HTTP_Client_Exception, "Auto", sTest_Exception)
{
    File::Binary lOF0(File::Folder::CURRENT, "Test_HTTP_Client_Exception_0.html", true);

    Net::Thread_Startup();

    HTTP::Client lC0(false);
    HTTP::Client lC1(true);

    // Get
    try
    {
        lC0.Get("https://localhost/index.htm", &lOF0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        lC1.Get("http://localhost/index.htm", &lOF0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        lC0.Get("http://localhost/index.htm", &lOF0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_SOCKET_CONNECT_FAILED");

    try
    {
        lC1.Get("https://localhost/index.htm", &lOF0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_SOCKET_CONNECT_FAILED");

    Net::Thread_Cleanup();
}
