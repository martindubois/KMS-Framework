
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/HTTP_Client.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/HTTP/Client.h>

using namespace KMS;

KMS_TEST(HTTP_Client_Base, "Auto", sTest_Base)
{
    File::Binary lOF0(File::Folder::CURRENT, "Test_HTTP_Client_Base_0.html", true);
    File::Binary lOF1(File::Folder::CURRENT, "Test_HTTP_Client_Base_1.zip" , true);

    Net::Thread_Startup();

    // Constructor
    HTTP::Client lC0;

    // Get
    lC0.Get("http://www.kms-quebec.com/MaPorteDuWEB/index.htm", &lOF0);

    Net::Thread_Cleanup();
}
