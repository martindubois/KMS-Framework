
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Simulator.h
// Library   KMS-C

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/DI/Array_Sparse.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Value.h>
#include <KMS/Modbus/Slave.h>
#include <KMS/Msg/IReceiver.h>

namespace KMS
{
    namespace Modbus
    {

        class Simulator : public DI::Dictionary, public Msg::IReceiver
        {

        public:

            static const unsigned int FLAG_VERBOSE_CHANGE;
            static const unsigned int FLAG_VERBOSE_READ;
            static const unsigned int FLAG_VERBOSE_WRITE;

            static int Main(int aCount, const char** aVector);

            Simulator();

            void InitSlave(Slave* aSlave);

            // aFlags  FLAG_VERBOSE_READ, FLAG_VERBOSE_WRITE, FLAG_VERBOSE_WRITE_CHANGE
            void AddCoil(const char* aN, Address aA, bool aValue, unsigned int aFlags = 0);

            // aFlags  FLAG_VERBOSE_READ
            void AddDiscreteInput(const char* aN, Address aA, bool aValue, unsigned int aFlags = 0);

            // aFlags  FLAG_VERBOSE_WRITE, FLAG_VERBOSE_WRITE_CHANGE
            void AddHoldingRegister(const char* aN, Address aA, RegisterValue aValue, unsigned int aFlags = 0);

            // aFlags  FLAG_VERBOSE_READ
            void AddInputRegister(const char* aN, Address aA, RegisterValue aValue, unsigned int aFlags = 0);

            int Run();

            void Stop();

            // ===== Msg::IReceiver =============================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        // Internal

            class Item : public DI::Value
            {

            public:

                Item();

                // ===== DI::Value ==========================================
                virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
                virtual void Set(const char* aIn);

                // ===== DI::Object =========================================
                virtual ~Item();

                unsigned int  mFlags;
                std::string   mName;
                RegisterValue mValue;

            };

        private:

            // ===== Message handlers =======================================
            unsigned int OnReadCoils           (Slave::MsgData* aData);
            unsigned int OnReadDiscreteInputs  (Slave::MsgData* aData);
            unsigned int OnReadHoldingRegisters(Slave::MsgData* aData);
            unsigned int OnReadInputRegisters  (Slave::MsgData* aData);
            unsigned int OnWriteSingleCoil     (Slave::MsgData* aData);
            unsigned int OnWriteSingleRegister (Slave::MsgData* aData);

            Slave* mSlave;

            // ===== Configurable attributes ================================
            DI::Array_Sparse mCoils;
            DI::Array_Sparse mDiscreteInputs;
            DI::Array_Sparse mHoldingRegisters;
            DI::Array_Sparse mInputRegisters;

        };

    }
}
