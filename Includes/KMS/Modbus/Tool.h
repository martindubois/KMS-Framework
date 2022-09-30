
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Tool.h
// Library   KMS-C

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/Modbus/Modbus.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Modbus
    {

        class Master;
        
        class Tool : public DI::Dictionary
        {

        public:

            static int Main(int aCount, const char** aVector);

            Tool();

            void InitMaster(Master* aMaster);

            void AddCoil           (const char* aN, uint16_t aA);
            void AddDiscreteInput  (const char* aN, uint16_t aA);
            void AddHoldingRegister(const char* aN, uint16_t aA);
            void AddInputRegister  (const char* aN, uint16_t aA);

            void AddOperation(const char* aO);

            void Connect();

            void Disconnect();

            void Dump(FILE* aOut);

            void ExecuteOperation(const char* aO);

            int Run();

            // ===== Modbus functions ===========================================

            bool ReadCoil(const char* aName);

            bool ReadDiscreteInput(const char* aName);

            uint16_t ReadHoldingRegister(const char* aName);

            uint16_t ReadInputRegister(const char* aName);

            void WriteSingleCoil(const char* aName, bool aValue);

            void WriteSingleRegister(const char* aName, RegisterValue aValue);

        private:

            Tool(const Tool&);

            const Tool& operator = (const Tool&);

            Master* mMaster;

            // ===== Configurable attributes ====================================
            DI::Dictionary mCoils;
            DI::Dictionary mDiscreteInputs;
            DI::Dictionary mHoldingRegisters;
            DI::Dictionary mInputRegisters;
            DI::Array      mOperations;

        };

    }
}
