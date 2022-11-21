
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-ByteTool/KMS-ByteTool.cpp

// ===== Includes ===========================================================
#include <KMS/Banner.h>
#include <KMS/ByteTool.h>

using namespace KMS;

// ===== KMS-ByteTool =======================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-ByteTool");

    return ByteTool::Main(aCount, aVector);
}
