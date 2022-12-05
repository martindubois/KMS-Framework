
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/KMS-STM32/SPI.cpp

#include "Component.h"

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

SPI::SPI() : mSPI(NULL) {}

void SPI::Init(uint8_t aSPI)
{
    switch (aSPI)
    {
    case 0: RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; break;
    case 1: RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; break;
    case 2: RCC->APB1ENR |= RCC_APB1ENR_SPI3EN; break;

    // default: assert(false);
    }

    mSPI = sSPIs[aSPI];

    mSPI->CR1 |= SPI_CR1_CPOL | SPI_CR1_SSI | SPI_CR1_SSM;

    mSPI->CR2 &= ~ SPI_CR2_DS_Msk;
    mSPI->CR2 |= (15 << SPI_CR2_DS_Pos) | SPI_CR2_RXNEIE;

    mSPI->CR1 |= SPI_CR1_SPE;

    NVIC_EnableIRQ(sIRQs[aSPI]);
}

#define TX_WORD(W,F)                            \
    mSPI->DR = (W);                             \
    if (0 == ((F) & ISlave::FLAG_TX_LAST_WORD)) \
    {                                           \
        mSPI->CR2 |= SPI_CR2_TXEIE;             \
    }

void SPI::OnInterrupt()
{
    // assert(NULL != mSPI);

    uint32_t lSR = mSPI->SR;

    uint8_t  lFlags;
    uint16_t lWord;

    if (0 != (lSR & SPI_SR_TXE))
    {
        mSPI->CR2 &= ~ SPI_CR2_TXEIE;

        lFlags = mSlave->OnTxReady(&lWord);
        if (0 != (lFlags & ISlave::FLAG_TX_WORD))
        {
            TX_WORD(lWord, lFlags)
        }
    }

    if (0 != (lSR & SPI_SR_RXNE))
    {
        lWord = mSPI->DR;

        lFlags = mSlave->OnRxWord(&lWord);
        if (0 != (lFlags & ISlave::FLAG_TX_WORD))
        {
            TX_WORD(lWord, lFlags);
        }
    }
}

// ===== KMS::Embedded::SPI =================================================

void SPI::Tx(uint16_t aWord, uint8_t aFlags)
{
    // assert(NULL != mSPI);

    TX_WORD(aWord, aFlags);
}

// Level: ISR
// CRITICAL PATH  SPI slave connexion
void SPI::Slave_Connect(ISlave* aSlave)
{
    // assert(NULL != aSlave);

    // assert(NULL == mSlave);
    // assert(NULL != mSPI);

    uint16_t lWord;

    uint8_t lFlags = aSlave->OnConnect(&lWord);

    mSPI->DR = lWord;

    mSPI->CR1 &= ~ SPI_CR1_SSI;

    if (0 == (lFlags & ISlave::FLAG_TX_LAST_WORD))
    {
        mSPI->CR2 |= SPI_CR2_TXEIE;
    }

    mSlave = aSlave;
}

void SPI::Slave_Disconnect()
{
    // assert(NULL != mSlave);
    // assert(NULL != mSPI);

    mSPI->CR1 |= SPI_CR1_SSI;

    mSlave->OnDisconnect();

    mSlave = NULL;
}
