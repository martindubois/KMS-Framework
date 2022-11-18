
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/KMS-STM32/USART.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "USART.h"

// Variables
// //////////////////////////////////////////////////////////////////////////

static DMA_Channel_TypeDef* sDMAs[7] =
{ DMA1_Channel1, DMA1_Channel2, DMA1_Channel3, DMA1_Channel4, DMA1_Channel5, DMA1_Channel6, DMA1_Channel7 };

static USART_TypeDef* sUSARTs[3] = { USART1, USART2, USART3 };

// Public
// //////////////////////////////////////////////////////////////////////////

// TODO Let the user select the baudrate and other settings.

void USART::Init(uint8_t aUSART, uint8_t aDMA, uint32_t aClock_Hz)
{
    mDMA   = aDMA;
    mUSART = aUSART;

    USART_TypeDef* lUSART = sUSARTs[mUSART];

    // TODO Do we need ONEBIT mode ?
    lUSART->CR1 |= USART_CR1_RXNEIE | USART_CR1_TE;
    lUSART->CR3 |= USART_CR3_DMAT | USART_CR3_ONEBIT;
    lUSART->BRR  = aClock_Hz / 115200 + 1; // bps

    lUSART->GTPR &= ~ USART_GTPR_PSC_Msk;
    lUSART->GTPR |= 0x1 << USART_GTPR_PSC_Pos;

    lUSART->CR1 |= USART_CR1_UE;

    NVIC_EnableIRQ(static_cast<IRQn_Type>(DMA1_Channel1_IRQn + mDMA));
    NVIC_EnableIRQ(static_cast<IRQn_Type>(USART1_IRQn        + mUSART));
}

void USART::Rx_OnInterrupt()
{
    USART_TypeDef* lUSART = sUSARTs[mUSART];

    uint32_t lISR = lUSART->ISR;

    // QUESTION Do we need to clear the "frame error" ?
    if (0 != (lISR & USART_ISR_FE))
    {
        lUSART->ICR = USART_ICR_FECF;
    }

    if (0 != (lISR & USART_ISR_RXNE))
    {
        Rx_Signal(lUSART->RDR);
    }
}

void USART::Tx_OnInterrupt()
{
    uint32_t lISR = DMA1->ISR;

    if (0 != (lISR & (DMA_ISR_TCIF1 << (4 * mDMA))))
    {
        DMA_Channel_TypeDef* lDMA = sDMAs[mDMA];

        lDMA->CCR &= ~ DMA_CCR_EN;

        Tx_Signal();

        // QUESTION Do we need to clear the global flag ?
        DMA1->IFCR = (DMA_ISR_GIF1 | DMA_ISR_TCIF1) << (4 * mDMA);
    }
}

// ===== KMS::Embedded::USART ===============================================

void USART::Rx_Disable() { sUSARTs[mUSART]->CR1 &= ~ USART_CR1_RE; }
void USART::Rx_Enable () { sUSARTs[mUSART]->CR1 |=   USART_CR1_RE; }

void USART::Tx(const void* aData, uint16_t aSize_byte)
{
    DMA_Channel_TypeDef* lDMA   = sDMAs  [mDMA  ];
    USART_TypeDef*       lUSART = sUSARTs[mUSART];

    lDMA->CPAR  = reinterpret_cast<uint32_t>(&lUSART->TDR);
    lDMA->CMAR  = reinterpret_cast<uint32_t>(aData);
    lDMA->CNDTR = aSize_byte;
    lDMA->CCR  |= DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE;
    
    DMA1->IFCR |= DMA_ISR_TCIF1 << (4 * mDMA);

    lUSART->ICR |= USART_ICR_TCCF;

    lDMA->CCR |= DMA_CCR_EN;

    KMS::Embedded::USART::Tx();
}

// Protected
// //////////////////////////////////////////////////////////////////////////

// ===== KMS::Embedded::USART ===============================================
void USART::Rx_DisableInterrupt() { sUSARTs[mUSART]->CR1 &= ~ USART_CR1_RXNEIE; }
void USART::Rx_EnableInterrupt () { sUSARTs[mUSART]->CR1 |=   USART_CR1_RXNEIE; }
