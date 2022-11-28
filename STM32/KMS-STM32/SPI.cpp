
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/KMS-STM32/SPI.cpp

#include "Component.h"

// ===== STM ================================================================
#include <stm32f3xx.h>

// ===== Local ==============================================================
#include "SPI.h"

using namespace KMS;

// Variables
// //////////////////////////////////////////////////////////////////////////

#define SPI_QTY (3)

static IRQn_Type    sIRQs[SPI_QTY] = { SPI1_IRQn, SPI2_IRQn, SPI1_IRQn };
static SPI_TypeDef* sSPIs[SPI_QTY] = { SPI1     , SPI2     , SPI3      };

// Public
// //////////////////////////////////////////////////////////////////////////

void SPI::Init(uint8_t aSPI)
{
    // assert(SPY_QTY > aSPI);

    mSPI = aSPI;

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    lSPI->CR1 |= SPI_CR1_SSM | (7 << SPI_CR1_BR_Pos);

    lSPI->CR2 &= ~ SPI_CR2_DS_Msk;
    lSPI->CR2 |= (7 << SPI_CR2_DS_Pos) | SPI_CR2_TXEIE | SPI_CR2_RXNEIE;

    lSPI->CR1 |= SPI_CR1_SPE;

    NVIC_EnableIRQ(sIRQs[mSPI]);
}

void SPI::OnInterrupt()
{
    // assert(SPY_QTY > mSPI);

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    uint32_t lSR = lSPI->SR;

    if (0 != (lSR & SPI_SR_TXE))
    {
        Tx_Signal();
    }

    if (0 != (lSR & SPI_SR_RXNE))
    {
        Rx_Signal(lSPI->DR);
    }
}

// ===== KMS::Embedded::SPI =================================================

void SPI::Tx(uint8_t aByte)
{
    // assert(SPY_QTY > mSPI);

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    Embedded::SPI::Tx();

    lSPI->DR = aByte;
}

void SPI::Slave_Connect(KMS::Msg::IReceiver* aReceiver, unsigned int aRx, unsigned int aTx)
{
    // assert(SPY_QTY > mSPI);

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    Embedded::SPI::Slave_Connect(aReceiver, aRx, aTx);

    lSPI->CR1 |= SPI_CR1_SSI;
}

void SPI::Slave_Disconnect()
{
    // assert(SPY_QTY > mSPI);

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    lSPI->CR1 &= ~ SPI_CR1_SSI;

    Embedded::SPI::Slave_Disconnect();
}
