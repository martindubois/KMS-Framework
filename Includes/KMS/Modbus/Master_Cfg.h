
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Master_Cfg.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/UInt.h>

namespace KMS
{
    namespace Modbus
    {

        class Master;

        class Master_Cfg : public DI::Dictionary
        {

        public:

            Master_Cfg(Master* aMaster);

        private:

            NO_COPY(Master_Cfg);

            // ===== Configurable attributes ================================
            DI::UInt_Ptr<uint8_t> mDeviceAddress;
            DI::UInt_Ptr<uint8_t> mRetryCount;

        };

    }
}
