
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave_Cfg.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/UInt.h>
#include <KMS/Modbus/Slave.h>

namespace KMS
{
    namespace Modbus
    {

        /// \brief TODO Remove use of DI namespace
        class Slave_Cfg final : public DI::Dictionary
        {

        private:

            DI::UInt_Ptr<uint8_t> mDeviceAddress;

        public:

            Slave_Cfg(Slave* aSlave);

        private:

            NO_COPY(Slave_Cfg);

        };

    }
}
