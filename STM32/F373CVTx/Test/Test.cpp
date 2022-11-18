
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/Test/Test.cpp

// ===== C ==================================================================
#include <stdlib.h>

// ===== Includes ===========================================================
#include <KMS/Embedded/WorkLoop.h>
#include <KMS/Msg/IReceiver.h>

#include <KMS/STM/STM32F.h>

using namespace KMS;

#define MSG_KEY          (1)
#define MSG_RX_BYTE      (2)
#define MSG_TX_COMPLETED (3)

class Test : public Msg::IReceiver
{

public:

    int Main(Embedded::USART* aUSART, DAQ::IDigitalOutputs *aDOs);

    // ===== KMS::Msg::IReceiver ============================================
    unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

private:

    // ===== Events =========================================================
    unsigned int OnKey();
    unsigned int OnRxByte(void* aData);
    unsigned int OnTxCompleted();

    DAQ::IDigitalOutputs* mDOs;
    Embedded::USART     * mUSART;
};

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main()
{
    STM::STM32F lProcessor;
    Test        lTest;

    lProcessor.Clock_Config();

    lProcessor.IO_ConfigureInterrupt(KMS_STM_ID_PA(2), &lTest, MSG_KEY);

    lProcessor.IO_SetMode(KMS_STM_ID_PC(0), STM::STM32F::IO_Mode::DIGITAL_OUTPUT_OPEN_DRAIN);

    Embedded::USART* lUSART = lProcessor.USART_Get(1, KMS_STM_ID_PD(6), KMS_STM_ID_PD(5));

    return lTest.Main(lUSART, &lProcessor);
}

// Public
// //////////////////////////////////////////////////////////////////////////

int Test::Main(Embedded::USART* aUSART, DAQ::IDigitalOutputs *aDOs)
{
    // assert(NULL != aUSART);
    // assert(NULL != aDOs);

    mDOs   = aDOs;
    mUSART = aUSART;

    mUSART->mOnRxByte     .Set(this, MSG_RX_BYTE);
    mUSART->mOnTxCompleted.Set(this, MSG_TX_COMPLETED);

    mUSART->Rx_Enable();

    Embedded::WorkLoop lWorkLoop;

    lWorkLoop.AddItem(mUSART);

    mDOs->DO_Set(KMS_STM_ID_PC(0), false);

    lWorkLoop.Run();

    return 0;
}

// ===== KMS::Msg::IReceiver ================================================

unsigned int Test::Receive(void* aSender, unsigned int aCode, void* aData)
{
    unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

    switch (aCode)
    {
    case MSG_KEY         : lResult = OnKey(); break;
    case MSG_RX_BYTE     : lResult = OnRxByte(aData); break;
    case MSG_TX_COMPLETED: lResult = OnTxCompleted(); break;
    }

    return lResult;
}

// Private
// //////////////////////////////////////////////////////////////////////////

unsigned int Test::OnKey()
{
    // assert(NULL != mUSART);

    if (mUSART->Tx_IsReady())
    {
        mUSART->Tx("Key\r\n", 5);
    }

    return 0;
}

unsigned int Test::OnRxByte(void* aData)
{
    // assert(NULL != aData);

    // assert(NULL != mUSART);

    mDOs->DO_Set(KMS_STM_ID_PC(0), false);

    mUSART->Tx_Wait();

    char* lData = reinterpret_cast<char*>(aData);

    switch (*lData)
    {
    case '\0': mUSART->Tx("Zero\r\n" , 6); break;
    case 'A' : mUSART->Tx("Alice\r\n", 7); break;
    case 'B' : mUSART->Tx("Bob\r\n"  , 5); break;
    default  : mUSART->Tx("Other\r\n", 7);
    }

    return 0;
}

unsigned int Test::OnTxCompleted()
{
    // assert(NULL != mDOs);

    mDOs->DO_Set(KMS_STM_ID_PC(0), true);

    return 0;
}
