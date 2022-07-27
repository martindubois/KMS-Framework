
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Master_Com.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Com/Port.h>
#include <KMS/Modbus/Master.h>

namespace KMS
{
    namespace Modbus
    {

        class Master_Com : public Master
        {

        public:

            // ===== Master =================================================
            virtual void         Connect();
            virtual void         Disconnect();
            virtual unsigned int Request_A(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_B(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_C(Function aFunction, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_D(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_E(Function aFunction, void* aOut, unsigned int aOutSize_byte);
            virtual void         Request_F(Function aFunction, uint16_t aAddr, uint16_t aCount, const void* aIn, unsigned int aInSize_byte);
            virtual unsigned int Request_G(Function aFunction, const void* aIn, unsigned int aSize_byte, void* aOut, unsigned int aOutSize_byte);

            // ===== Cfg::Configurable ======================================
            virtual void InitConfigurator(Cfg::Configurator* aC);

        private:

            void Request_Send(Function aFunction, const void* aIn, unsigned int aInSize_byte);

            void VerifyDeviceAddress(const uint8_t* aData);

            Com::Port mPort;

        };

    }
}
