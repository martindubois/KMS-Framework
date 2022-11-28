
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/KMS-STM32/USART.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Embedded/USART.h>

class USART : public KMS::Embedded::USART
{

public:

    void Init(uint8_t aUSART, uint8_t aDMA, uint32_t aClock_Hz);

    void Rx_OnInterrupt();
    void Tx_OnInterrupt();

    // ===== KMS::Embedded::USART ===========================================
    virtual void Rx_Disable();
    virtual void Rx_Enable ();
    virtual void Tx(const void* aData, uint16_t aSize_byte);

protected:

    // ===== KMS::Embedded::USART ===========================================
    virtual void Rx_DisableInterrupt();
    virtual void Rx_EnableInterrupt ();

private:

    uint8_t mDMA;
    uint8_t mUSART;

};
