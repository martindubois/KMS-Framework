
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-ModbusTool/KMS-ModbusTool.cpp

#include <KMS/Base.h>

// ===== Includes ===========================================================
#include <KMS/Banner.h>
#include <KMS/Modbus/Tool.h>

using namespace KMS;

// ===== KMS-ByteTool =======================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-ModbusTool");

    return Modbus::Tool::Main(aCount, aVector);
}
