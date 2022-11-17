
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/Test/Test.cpp

// ===== C ==================================================================
#include <stdlib.h>

// ===== Includes ===========================================================
#include <KMS/STM/STM32F.h>

using namespace KMS;

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main()
{
    STM::STM32F lProcessor;

    lProcessor.Clock_Config();

    lProcessor.IO_SetMode(KMS_STM_ID_PC(0), STM::STM32F::IO_Mode::DIGITAL_OUTPUT_OPEN_DRAIN);

    Embedded::USART* lUSART = lProcessor.USART_Get(1, KMS_STM_ID_PD(6), KMS_STM_ID_PD(5));

    lProcessor.DO_Set(KMS_STM_ID_PC(0), true);

    bool lValue = true;

    for (;;)
    {
        if (lUSART->Tx_IsReady())
        {
            lUSART->Tx("Tx\r\n", 4);

            lValue = ! lValue;
            lProcessor.DO_Set(KMS_STM_ID_PC(0), lValue);
        }
    }
}
