
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Tool.h
// Library   KMS-C

#pragma once

// ===== Includes ===========================================================
#include <KMS/CLI/Tool.h>
#include <KMS/DI/Array.h>
#include <KMS/Modbus/Modbus.h>
#include <KMS/Scope/Channel_Modbus.h>
#include <KMS/Types.h>
#include <KMS/WGDI/Scope_Module.h>

namespace KMS
{
    namespace Modbus
    {

        class Master;
        
        class Tool final : public CLI::Tool
        {

        private:

            DI::Dictionary mCoils;
            DI::Dictionary mDiscreteInputs;
            DI::Dictionary mHoldingRegisters;
            DI::Dictionary mInputRegisters;

        public:

            static int Main(int aCount, const char** aVector);

            Tool();

            void InitMaster(Master* aMaster);

            void AddCoil           (const char* aN, uint16_t aA);
            void AddDiscreteInput  (const char* aN, uint16_t aA);
            void AddHoldingRegister(const char* aN, uint16_t aA);
            void AddInputRegister  (const char* aN, uint16_t aA);

            void Connect();

            void Disconnect();

            void Dump(FILE* aOut);

            // ===== Modbus functions ===========================================

            bool ReadCoil(const char* aName);

            bool ReadDiscreteInput(const char* aName);

            uint16_t ReadHoldingRegister(const char* aName);

            uint16_t ReadInputRegister(const char* aName);

            void WriteSingleCoil(const char* aName, bool aValue);

            void WriteSingleRegister(const char* aName, RegisterValue aValue);

            // ===== CLI::Tool ==============================================
            virtual void DisplayHelp(FILE* aOut) const;
            virtual int  ExecuteCommand(CLI::CommandLine* aCmd);
            virtual int  Run();

        private:

            NO_COPY(Tool);

            void Scope_Channel(Scope::Channel_Modbus* aChannel, const char* aAddrOrName, const DI::Dictionary& aMap);

            WGDI::Scope_Module mScope;

            Master* mMaster;

        };

    }
}
