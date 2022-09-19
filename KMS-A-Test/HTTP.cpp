
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/HTTP.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>
#include <KMS/DI/MetaData.h>
#include <KMS/HTTP/HTTP.h>

KMS_TEST(HTTP_Base, "HTTP_Base", "Auto", sTest_Base)
{
    char                lBuffer[1024];
    const char        * lIn;
    KMS::DI::Dictionary lD0;

    lD0.SetMetaData(&KMS::DI::META_DATA_DYNAMIC);

    // Dictionary - Empty
    lIn = "";
    KMS_TEST_COMPARE(KMS::HTTP::Decode_Dictionary(&lD0, lIn, 0), 0UL);
    KMS_TEST_COMPARE(KMS::HTTP::Encode_Dictionary(&lD0, lBuffer, sizeof(lBuffer)), 0UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
}
