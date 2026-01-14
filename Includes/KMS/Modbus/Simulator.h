
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Simulator.h
// Status
// Library   KMS-C

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/DI/Array_Sparse.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Value.h>
#include <KMS/Modbus/Slave.h>

namespace KMS
{
    namespace Modbus
    {

        class Simulator final : public DI::Dictionary
        {

        private:

            DI::Array_Sparse mCoils;
            DI::Array_Sparse mDiscreteInputs;
            DI::Array_Sparse mHoldingRegisters;
            DI::Array_Sparse mInputRegisters;

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

        // Internal

            class Item : public DI::Value
            {

            public:

                Item();

                // ===== DI::Value ==========================================
                virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
                virtual void Set(const char* aIn);
                virtual bool Set_Try(const char* aIn);

                // ===== DI::Object =========================================
                virtual ~Item();
                virtual bool Clear();

                unsigned int  mFlags;
                String_ASCII  mName;
                RegisterValue mValue;

            };

        private:

            // ===== Callbacks ==============================================
            Callback<Simulator> ON_READ_COILS;
            Callback<Simulator> ON_READ_DISCRETE_INPUTS;
            Callback<Simulator> ON_READ_HOLDING_REGISTERS;
            Callback<Simulator> ON_READ_INPUT_REGISTERS;
            Callback<Simulator> ON_WRITE_SINGLE_COIL;
            Callback<Simulator> ON_WRITE_SINGLE_REGISTER;
            unsigned int OnReadCoils           (void* aSender, void* aData);
            unsigned int OnReadDiscreteInputs  (void* aSender, void* aData);
            unsigned int OnReadHoldingRegisters(void* aSender, void* aData);
            unsigned int OnReadInputRegisters  (void* aSender, void* aData);
            unsigned int OnWriteSingleCoil     (void* aSender, void* aData);
            unsigned int OnWriteSingleRegister (void* aSender, void* aData);

            Slave* mSlave;

        };

    }
}
