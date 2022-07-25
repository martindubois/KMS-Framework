
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Simulator.h

#pragma once

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurable.h>
#include <KMS/Modbus/Slave.h>
#include <KMS/Msg/IReceiver.h>

namespace KMS
{
    namespace Modbus
    {

        class Simulator : public Cfg::Configurable, public Msg::IReceiver
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
            virtual bool Receive(void* aSender, unsigned int aCode, void* aData);

            // ===== Cfg::Configurable ==========================================
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual void DisplayHelp(FILE* aOut) const;
            virtual bool SetAttribute(const char* aA, const char* aV);

        // Internal

            class Item
            {

            public:

                Item(const char* aN, RegisterValue aV, unsigned int aF);

                unsigned int  mFlags;
                std::string   mName;
                RegisterValue mValue;

            };

            typedef std::map<Address, Item> ItemMap;

        private:

            // ===== Message handlers =======================================
            bool OnReadCoils           (Slave::MsgData* aData);
            bool OnReadDiscreteInputs  (Slave::MsgData* aData);
            bool OnReadHoldingRegisters(Slave::MsgData* aData);
            bool OnReadInputRegisters  (Slave::MsgData* aData);
            bool OnWriteSingleCoil     (Slave::MsgData* aData);
            bool OnWriteSingleRegister (Slave::MsgData* aData);

            Slave* mSlave;

            // ===== Configurable attributes ================================
            ItemMap mCoils;
            ItemMap mDiscreteInputs;
            ItemMap mHoldingRegisters;
            ItemMap mInputRegisters;

        };

    }
}
