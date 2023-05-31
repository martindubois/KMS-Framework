
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-ModbusSim/KMS-ModbusSim.cpp

#include <KMS/Base.h>

// ===== Includes ===========================================================
#include <KMS/Banner.h>
#include <KMS/Exception.h>
#include <KMS/Modbus/Simulator.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

using namespace KMS;

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-ModbusSim");

    return Modbus::Simulator::Main(aCount, aVector);
}
