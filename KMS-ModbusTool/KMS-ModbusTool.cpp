
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-ModbusTool/KMS-ModbusTool.cpp

#include <KMS/Base.h>

// ===== Includes ===========================================================
#include <KMS/Banner.h>
#include <KMS/Modbus/Tool.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

using namespace KMS;

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-ModbusTool");

    return Modbus::Tool::Main(aCount, aVector);
}
