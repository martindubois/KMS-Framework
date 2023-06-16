
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave_Cfg.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Di/Dictionary.h>
#include <KMS/DI/UInt_Wrapper.h>
#include <KMS/Modbus/Slave.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave_Cfg : public DI::Dictionary
        {

        public:

            Slave_Cfg(Slave* aSlave);

        private:

            NO_COPY(Slave_Cfg);

            // ===== Configurable attributes ================================
            DI::UInt_Wrapper<uint8_t> mDeviceAddress_Wrapper;

        };

    }
}
