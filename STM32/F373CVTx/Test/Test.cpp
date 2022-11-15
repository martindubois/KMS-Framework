
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/Test/Test.cpp

// ===== Includes ===========================================================
#include <KMS/STM/STM32F.h>

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main()
{
    KMS::STM::STM32F lProcessor;

    lProcessor.IO_SetMode(KMS::STM::STM32F::ID_PC0, KMS::STM::STM32F::IO_Mode::DIGITAL_OUTPUT);

    bool lValue = false;

    for (;;)
    {
        lProcessor.DO_Set(KMS::STM::STM32F::ID_PC0, lValue);

        lValue = ! lValue;
    }
}
