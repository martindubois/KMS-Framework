
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/HTTP.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/MetaData.h>
#include <KMS/HTTP/HTTP.h>

KMS_TEST(HTTP_Base, "HTTP_Base", "Auto", sTest_Base)
{
    char                lBuffer[1024];
    KMS::DI::Dictionary lD0;
    const char        * lIn;
    unsigned int        lSize_byte;

    // Dictionary - Empty
    lIn = "\r\n";
    KMS_TEST_COMPARE(KMS::HTTP::Decode_Dictionary(&lD0, lIn, 2), 2UL);
    KMS_TEST_COMPARE(KMS::HTTP::Encode_Dictionary(&lD0, lBuffer, sizeof(lBuffer)), 2UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));

    // Dictionary
    lD0.Clear();
    lIn = "Host: localhost\r\n"
          "Connection : keep - alive\r\n"
          "sec - ch - ua : \"Microsoft Edge\"; v = \"105\", \" Not;A Brand\"; v = \"99\", \"Chromium\"; v = \"105\"\r\n"
          "sec - ch - ua - mobile: ? 0\r\n"
          "sec - ch - ua - platform : \"Windows\"\r\n"
          "Upgrade - Insecure - Requests : 1\r\n"
          "User - Agent : Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 105.0.0.0 Safari / 537.36 Edg / 105.0.1343.42\r\n"
          "Accept : text / html, application / xhtml + xml, application / xml; q = 0.9, image / webp, image / apng, */*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
          "Sec-Fetch-Site: none\r\n"
          "Sec-Fetch-Mode: navigate\r\n"
          "Sec-Fetch-User: ?1\r\n"
          "Sec-Fetch-Dest: document\r\n"
          "Accept-Encoding: gzip, deflate, br\r\n"
          "Accept-Language: en-US,en;q=0.9\r\n"
          "\r\n";
    lSize_byte = static_cast<unsigned int>(strlen(lIn));
    KMS_TEST_COMPARE(KMS::HTTP::Decode_Dictionary(&lD0, lIn, lSize_byte), lSize_byte);
    KMS_TEST_COMPARE(KMS::HTTP::Encode_Dictionary(&lD0, lBuffer, sizeof(lBuffer)), lSize_byte);
}
