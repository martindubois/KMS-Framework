
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

static IRQn_Type    sIRQs[SPI_QTY] = { SPI1_IRQn, SPI2_IRQn, SPI3_IRQn };
static SPI_TypeDef* sSPIs[SPI_QTY] = { SPI1     , SPI2     , SPI3      };

// Public
// //////////////////////////////////////////////////////////////////////////

SPI::SPI() : mSPI(0xff) {}

void SPI::Init(uint8_t aSPI)
{
    // assert(SPY_QTY > aSPI);

    switch (aSPI)
    {
    case 0: RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; break;
    case 1: RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; break;
    case 2: RCC->APB1ENR |= RCC_APB1ENR_SPI3EN; break;
    }

    mSPI = aSPI;

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    lSPI->CR1 |= SPI_CR1_CPOL | SPI_CR1_SSI | SPI_CR1_SSM;

    lSPI->CR2 &= ~ SPI_CR2_DS_Msk;
    lSPI->CR2 |= (15 << SPI_CR2_DS_Pos) | SPI_CR2_RXNEIE;

    lSPI->CR1 |= SPI_CR1_SPE;

    NVIC_EnableIRQ(sIRQs[mSPI]);
}

#define TX_WORD(S,W,F)                          \
    (S)->DR = (W);                              \
    if (0 == ((F) & ISlave::FLAG_TX_LAST_WORD)) \
    {                                           \
        (S)->CR2 |= SPI_CR2_TXEIE;              \
    }

void SPI::OnInterrupt()
{
    // assert(SPY_QTY > mSPI);

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    uint32_t lSR = lSPI->SR;

    uint8_t  lFlags;
    uint16_t lWord;

    if (0 != (lSR & SPI_SR_TXE))
    {
        lSPI->CR2 &= ~ SPI_CR2_TXEIE;

        lFlags = mSlave->OnTxReady(&lWord);
        if (0 != (lFlags & ISlave::FLAG_TX_WORD))
        {
            TX_WORD(lSPI, lWord, lFlags)
        }
    }

    if (0 != (lSR & SPI_SR_RXNE))
    {
        lWord = lSPI->DR;

        lFlags = mSlave->OnRxWord(&lWord);
        if (0 != (lFlags & ISlave::FLAG_TX_WORD))
        {
            TX_WORD(lSPI, lWord, lFlags);
        }
    }
}

// ===== KMS::Embedded::SPI =================================================

void SPI::Tx(uint16_t aWord, uint8_t aFlags)
{
    // assert(SPY_QTY > mSPI);

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    TX_WORD(lSPI, aWord, aFlags);
}

void SPI::Slave_Connect(ISlave* aSlave)
{
    // assert(NULL != aSlave);

    // assert(NULL == mSlave);
    // assert(SPY_QTY > mSPI);

    mSlave = aSlave;

    uint16_t lWord;

    uint8_t lFlags = mSlave->OnConnect(&lWord);

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    if (0 != (lFlags & ISlave::FLAG_TX_WORD))
    {
        TX_WORD(lSPI, lWord, lFlags);
    }

    lSPI->CR1 &= ~ SPI_CR1_SSI;
}

void SPI::Slave_Disconnect()
{
    // assert(NULL != mSlave);
    // assert(SPY_QTY > mSPI);

    SPI_TypeDef* lSPI = sSPIs[mSPI];

    lSPI->CR1 |= SPI_CR1_SSI;

    mSlave->OnDisconnect();

    mSlave = NULL;
}
