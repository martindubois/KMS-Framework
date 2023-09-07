
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/HTTP/Server.h>

using namespace KMS;

KMS_TEST(HTTP_Server_Base, "Auto", sTest_Base)
{
    HTTP::Server lS;
}
