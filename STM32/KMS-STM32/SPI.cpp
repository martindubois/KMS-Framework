
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

// Macros
// //////////////////////////////////////////////////////////////////////////

#define SET_WORD_SIZE(S)                       \
    mSPI->CR2 &= ~ SPI_CR2_DS_Msk;             \
    mSPI->CR2 |= (((S) - 1) << SPI_CR2_DS_Pos)

#define SET_WORD_SIZE_16()               \
    mSPI->CR2 |= (0xf << SPI_CR2_DS_Pos)

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

    default: Assert(false);
    }

    mSPI = sSPIs[aSPI];

    mSPI->CR1 |= SPI_CR1_CPOL | SPI_CR1_SSI | SPI_CR1_SSM;

    SET_WORD_SIZE_16();

    mSPI->CR2 |= SPI_CR2_RXNEIE;

    mSPI->CR1 |= SPI_CR1_SPE;

    NVIC_EnableIRQ(sIRQs[aSPI]);
}

void SPI::OnInterrupt()
{
    uint32_t lSR = mSPI->SR;

    uint8_t  lFlags;
    uint16_t lWord;

    if ((0 != (lSR & SPI_SR_TXE)) && (0 != (mSPI->CR2 & SPI_CR2_TXEIE)))
    {
        lFlags = mSlave->OnTxReady(&lWord);
        if (0 != (lFlags & ISlave::FLAG_TX_WORD))
        {
            mSPI->DR = lWord;
            if (0 != (lFlags & ISlave::FLAG_TX_LAST_WORD))
            {
                mSPI->CR2 &= ~ SPI_CR2_TXEIE;
            }
        }
        else
        {
            mSPI->CR2 &= ~ SPI_CR2_TXEIE;
        }
    }

    while (0 != (lSR & SPI_SR_RXNE))
    {
        lWord = mSPI->DR;

        lFlags = mSlave->OnRxWord(&lWord);
        if (0 != (lFlags & ISlave::FLAG_TX_WORD))
        {
            mSPI->DR = lWord;
            if (0 == (lFlags & ISlave::FLAG_TX_LAST_WORD))
            {
                mSPI->CR2 |= SPI_CR2_TXEIE;
            }
        }

        lSR = mSPI->SR;
    }
}

// ===== KMS::Embedded::SPI =================================================

// CRITICAL PATH  SPI slave connexion
void SPI::Slave_Connect(ISlave* aSlave)
{
    // assert(NULL != aSlave);

    mSlave = aSlave;

    uint16_t lWord;

    uint8_t lFlags = aSlave->OnConnect(&lWord);

    if (0 != (lFlags & (ISlave::FLAG_CLOCK_LOW)))
    {
        mSPI->CR1 &= ~ SPI_CR1_CPOL;
    }

    switch (lFlags & ISlave::FLAG_WORD_09)
    {
    case ISlave::FLAG_WORD_09: SET_WORD_SIZE(9); break;
    }

    mSPI->DR = lWord;

    mSPI->CR1 &= ~ SPI_CR1_SSI;

    if (0 == (lFlags & ISlave::FLAG_TX_LAST_WORD))
    {
        mSPI->CR2 |= SPI_CR2_TXEIE;
    }
}

// This method is responsible for restoring default configuration. This make
// the Slave_Connect faster. It
// - Reset the default clock polarity
// - Reset the word size to 16 bits

// This method does not return mSlave to NULL because some time a SPI
// interrupt is processed after the processing of the chip select interrupt.

void SPI::Slave_Disconnect()
{
    Assert(NULL != mSlave);

    uint32_t lSR = mSPI->SR;

    while (0 != (lSR & SPI_SR_RXNE))
    {
        uint16_t lWord = mSPI->DR;

        uint8_t lFlags = mSlave->OnRxWord(&lWord);
        Assert(0 == lFlags);
        (void)lFlags;

        lSR = mSPI->SR;
    }

    mSPI->CR1 |= SPI_CR1_CPOL | SPI_CR1_SSI;
    mSPI->CR2 &= ~ SPI_CR2_TXEIE;

    SET_WORD_SIZE_16();
}
