
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-ComTool/KMS-ComTool.cpp

// ===== Includes ===========================================================
#include <KMS/Banner.h>
#include <KMS/Com/Tool.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-ComTool");

    return KMS::Com::Tool::Main(aCount, aVector);
}
