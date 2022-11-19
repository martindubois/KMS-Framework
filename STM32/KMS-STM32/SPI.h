
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/KMS-STM32/SPI.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Embedded/SPI.h>

class SPI : public KMS::Embedded::SPI
{

public:

    void Init(uint8_t aSPI);

    void OnInterrupt();

    // ===== KMS::Embedded::SPI =============================================
    virtual void Tx(uint8_t aByte);
    virtual void Slave_Connect(KMS::Msg::IReceiver* aReceiver, unsigned int aRx, unsigned int aTx);
    virtual void Slave_Disconnect();

private:

    uint8_t mSPI;

};
