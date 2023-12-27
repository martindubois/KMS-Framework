
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Scope/Channel_Modbus.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Modbus/Master.h>
#include <KMS/Scope/Channel.h>

namespace KMS
{
    namespace Scope
    {

        class Channel_Modbus : public Channel
        {

        public:

            void Init(Modbus::Master* aMaster, Modbus::Address aAddress);

        protected:

            Channel_Modbus();

            Modbus::Address GetAddress() const;

            Modbus::Master* GetMaster();

        private:

            Modbus::Address mAddress;
            Modbus::Master* mMaster;

        };

        class Channel_Modbus_Coil final : public Channel_Modbus
        {

        public:

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);


        };

        class Channel_Modbus_DiscreteInput final : public Channel_Modbus
        {

        public:

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);


        };

        class Channel_Modbus_HoldingRegister final : public Channel_Modbus
        {

        public:

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);

        };

        class Channel_Modbus_InputRegister final : public Channel_Modbus
        {

        public:

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);

        };

    }
}