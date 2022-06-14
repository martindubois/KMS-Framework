
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Make/KMS-Make.cpp

// ===== Includes ===========================================================
#include <KMS/Banner.h>
#include <KMS/Build/Make.h>

// ===== KMS-ByteTool =======================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-Make");

    return KMS::Build::Make::Main(aCount, aVector);
}
