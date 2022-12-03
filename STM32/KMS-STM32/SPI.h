
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/KMS-STM32/SPI.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Embedded/SPI.h>

class SPI : public KMS::Embedded::SPI
{

public:

    SPI();

    void Init(uint8_t aSPI);

    void OnInterrupt();

    // ===== KMS::Embedded::SPI =============================================
    virtual void Tx(uint16_t aWord);
    virtual void Slave_Connect(IDevice* aDevice);
    virtual void Slave_Disconnect();

private:

    uint8_t mSPI;

};
