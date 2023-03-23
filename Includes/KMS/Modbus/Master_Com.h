
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

            Master_Com();

            Com::Port* GetPort();

            // ===== Master =================================================
            virtual void         Connect();
            virtual void         Disconnect();

        protected:

            // ===== Master =================================================
            virtual unsigned int Request_A(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_B(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_C(Function aFunction, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_D(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_E(Function aFunction, void* aOut, unsigned int aOutSize_byte);
            virtual void         Request_F(Function aFunction, uint16_t aAddr, uint16_t aCount, const void* aIn, unsigned int aInSize_byte);
            virtual unsigned int Request_G(Function aFunction, const void* aIn, unsigned int aSize_byte, void* aOut, unsigned int aOutSize_byte);

        private:

            void Request_Send(Function aFunction, const void* aIn, unsigned int aInSize_byte);

            Com::Port mPort;

        };

    }
}
