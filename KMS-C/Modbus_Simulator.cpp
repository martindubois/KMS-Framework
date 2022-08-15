
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Simulator.cpp

#include "Component.h"

// ===== C ==================================================================
#include <signal.h>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Console/Color.h>
#include <KMS/Convert.h>
#include <KMS/Modbus/Slave_Com.h>

#include <KMS/Modbus/Simulator.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-ModbusSim.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_READ_COILS             (1)
#define MSG_READ_DISCRETE_INPUTS   (2)
#define MSG_READ_HOLDING_REGISTERS (3)
#define MSG_READ_INPUT_REGISTERS   (4)
#define MSG_WRITE_SINGLE_COIL      (5)
#define MSG_WRITE_SINGLE_REGISTER  (6)

// Static variable
// //////////////////////////////////////////////////////////////////////////

static KMS::Modbus::Simulator* sSimulator = NULL;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

extern "C"
{
    static void OnCtrlC(int aSignal);
}

static void AddItem(KMS::Modbus::Simulator::ItemMap * aMap, const char* aV);

static KMS::Modbus::RegisterValue ToRegisterValue(const char* aIn);

static void TraceKnown(const char* aOp, KMS::Modbus::Address aA, const KMS::Modbus::Simulator::Item & aItem, unsigned int aFlags);

static void TraceUnknown(const char* aOp, KMS::Modbus::Address aA, KMS::Modbus::RegisterValue aV);

namespace KMS
{

    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int Simulator::FLAG_VERBOSE_CHANGE = 0x00000001;
        const unsigned int Simulator::FLAG_VERBOSE_READ   = 0x00000002;
        const unsigned int Simulator::FLAG_VERBOSE_WRITE  = 0x00000004;

        int Simulator::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);
            assert(NULL != aVector[0]);

            int lResult = __LINE__;

            _crt_signal_t lHandler = NULL;

            try
            {
                KMS::Cfg::Configurator lC;
                Slave_Com lSl;
                Simulator lSi;

                lSl.InitConfigurator(&lC);
                lSi.InitConfigurator(&lC);
                lSi.InitSlave(&lSl);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::HOME      ), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                sSimulator = &lSi;

                lHandler = signal(SIGINT, OnCtrlC);

                std::cout << "Press Ctrl-C to stop" << std::endl;

                lResult = lSi.Run();

            }
            KMS_CATCH_RESULT(lResult)

            signal(SIGINT, lHandler);

            sSimulator = NULL;

            return lResult;
        }

        Simulator::Simulator() : mSlave(NULL) {}

        void Simulator::InitSlave(Slave* aSlave)
        {
            assert(NULL != aSlave);

            mSlave = aSlave;

            mSlave->mOnReadCoils           .Set(this, MSG_READ_COILS);
            mSlave->mOnReadDiscreteInputs  .Set(this, MSG_READ_DISCRETE_INPUTS);
            mSlave->mOnReadHoldingRegisters.Set(this, MSG_READ_HOLDING_REGISTERS);
            mSlave->mOnReadInputRegisters  .Set(this, MSG_READ_INPUT_REGISTERS);
            mSlave->mOnWriteSingleCoil     .Set(this, MSG_WRITE_SINGLE_COIL);
            mSlave->mOnWriteSingleRegister .Set(this, MSG_WRITE_SINGLE_REGISTER);
        }

        int Simulator::Run()
        {
            assert(NULL != mSlave);

            mSlave->Connect();
            mSlave->Run();

            return 0;
        }

        void Simulator::Stop() { assert(NULL != mSlave); mSlave->Stop(); }

        // ===== Msg::IReceiver =============================================

        unsigned int Simulator::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            Slave::MsgData* lData = reinterpret_cast<Slave::MsgData*>(aData);

            unsigned int lResult;

            switch (aCode)
            {
            case MSG_READ_COILS            : lResult = OnReadCoils           (lData); break;
            case MSG_READ_DISCRETE_INPUTS  : lResult = OnReadDiscreteInputs  (lData); break;
            case MSG_READ_HOLDING_REGISTERS: lResult = OnReadHoldingRegisters(lData); break;
            case MSG_READ_INPUT_REGISTERS  : lResult = OnReadInputRegisters  (lData); break;
            case MSG_WRITE_SINGLE_COIL     : lResult = OnWriteSingleCoil     (lData); break;
            case MSG_WRITE_SINGLE_REGISTER : lResult = OnWriteSingleRegister (lData); break;

            default:
                assert(false);
                lResult = KMS::Msg::IReceiver::MSG_IGNORED;
            }

            return lResult;
        }

        // ====== Cfg::Configurable =========================================

        bool Simulator::AddAttribute(const char* aA, const char* aV)
        {
            CFG_IF("Coils"           ) { AddItem(&mCoils           , aV); return true; }
            CFG_IF("DiscreteInputs"  ) { AddItem(&mDiscreteInputs  , aV); return true; }
            CFG_IF("HoldingRegisters") { AddItem(&mHoldingRegisters, aV); return true; }
            CFG_IF("InputRegisters"  ) { AddItem(&mInputRegisters  , aV); return true; }

            return Cfg::Configurable::AddAttribute(aA, aV);
        }

        void Simulator::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "===== KMS::Modbus::Simulator =====\n"
                "Coils\n"
                "    Clear the coils list\n"
                "Coils += {Address};{Name}[;{Value}[;{Flags}]]\n"
                "    Add a coil\n"
                "DiscreteInputs\n"
                "    Clear the discrete input list\n"
                "DiscreteInputs += {Address};{Name}[;{Value}[;{Flags}]]\n"
                "    Add a discret input\n"
                "HoldingRegisters\n"
                "    Clear the holding register list\n"
                "HoldingRegisters += {Address};{Name}[;{Value}[;{Flags}]]\n"
                "    Add a holding register\n"
                "InputRegisters\n"
                "    Clear the input register list\n"
                "InputRegisters += {Address};{Name}[;{Value}[;{Flags}]]\n"
                "    Add an input register\n");

            Cfg::Configurable::DisplayHelp(aOut);
        }

        bool Simulator::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Coils"           ) { mCoils           .clear(); return true; }
                CFG_IF("DiscreteInputs"  ) { mDiscreteInputs  .clear(); return true; }
                CFG_IF("HoldingRegisters") { mHoldingRegisters.clear(); return true; }
                CFG_IF("InputRegisters"  ) { mInputRegisters  .clear(); return true; }
            }

            return Cfg::Configurable::SetAttribute(aA, aV);
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Simulator::Item::Item(const char* aN, RegisterValue aV, unsigned int aF) : mFlags(aF), mName(aN), mValue(aV)
        {
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Message handlers ===========================================

        unsigned int Simulator::OnReadCoils(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            for (unsigned int i = 0; i < aData->mQty; i++)
            {
                bool lValue = false;

                ItemMap::iterator lIt = mCoils.find(aData->mStartAddr + i);
                if (mCoils.end() == lIt)
                {
                    TraceUnknown("Read Coil", aData->mStartAddr + i, OFF);
                }
                else
                {
                    TraceKnown("Read Coil", aData->mStartAddr + i, lIt->second, FLAG_VERBOSE_READ);

                    lValue = lIt->second.mValue;
                }

                WriteBit(aData->mBuffer, 0, i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnReadDiscreteInputs(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            for (unsigned int i = 0; i < aData->mQty; i++)
            {
                bool lValue = false;

                ItemMap::iterator lIt = mDiscreteInputs.find(aData->mStartAddr + i);
                if (mDiscreteInputs.end() == lIt)
                {
                    TraceUnknown("Read Discrete Input", aData->mStartAddr + i, OFF);
                }
                else
                {
                    TraceKnown("Read Discrete Input", aData->mStartAddr + i, lIt->second, FLAG_VERBOSE_READ);

                    lValue = lIt->second.mValue;
                }

                WriteBit(aData->mBuffer, 0, i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnReadHoldingRegisters(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            for (unsigned int i = 0; i < aData->mQty; i++)
            {
                RegisterValue lValue = 0;

                ItemMap::iterator lIt = mHoldingRegisters.find(aData->mStartAddr + i);
                if (mHoldingRegisters.end() == lIt)
                {
                    TraceUnknown("Read Holding Register", aData->mStartAddr + i, 0);
                }
                else
                {
                    TraceKnown("Read Holding Register", aData->mStartAddr + i, lIt->second, FLAG_VERBOSE_READ);

                    lValue = lIt->second.mValue;
                }

                WriteUInt16(aData->mBuffer, sizeof(RegisterValue) * i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnReadInputRegisters(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            for (unsigned int i = 0; i < aData->mQty; i++)
            {
                RegisterValue lValue = 0;

                ItemMap::iterator lIt = mInputRegisters.find(aData->mStartAddr + i);
                if (mHoldingRegisters.end() == lIt)
                {
                    TraceUnknown("Read Input Register", aData->mStartAddr + i, 0);
                }
                else
                {
                    TraceKnown("Read Input Register", aData->mStartAddr, lIt->second, FLAG_VERBOSE_READ);

                    lValue = lIt->second.mValue;
                }

                WriteUInt16(aData->mBuffer, sizeof(RegisterValue) * i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnWriteSingleCoil(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            RegisterValue lValue = ReadUInt16(aData->mBuffer, 0);

            ItemMap::iterator lIt = mCoils.find(aData->mStartAddr);
            if (mCoils.end() == lIt)
            {
                TraceUnknown("Write Single Coil", aData->mStartAddr, lValue);
            }
            else
            {
                bool lBool = ON == lValue;
                unsigned int lFlags = FLAG_VERBOSE_WRITE;

                if ((0 != lIt->second.mValue) != lBool)
                {
                    lIt->second.mValue = lBool;
                    lFlags |= FLAG_VERBOSE_CHANGE;
                }

                TraceKnown("Write Single Coil", aData->mStartAddr, lIt->second, lFlags);
            }

            return 0;
        }

        unsigned int Simulator::OnWriteSingleRegister(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            RegisterValue lValue = ReadUInt16(aData->mBuffer, 0);

            ItemMap::iterator lIt = mCoils.find(aData->mStartAddr);
            if (mCoils.end() == lIt)
            {
                TraceUnknown("Write Single Register", aData->mStartAddr, lValue);
            }
            else
            {
                unsigned int lFlags = FLAG_VERBOSE_WRITE;

                if (lIt->second.mValue != lValue)
                {
                    lIt->second.mValue = lValue;
                    lFlags |= FLAG_VERBOSE_CHANGE;
                }

                TraceKnown("Write Single Register", aData->mStartAddr, lIt->second, lFlags);
            }

            return 0;
        }

    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void OnCtrlC(int aSignal)
{
    assert(SIGINT == aSignal);

    assert(NULL != sSimulator);

    std::cout << "Ctrl-C" << std::endl;

    sSimulator->Stop();
}

void AddItem(KMS::Modbus::Simulator::ItemMap* aMap, const char* aV)
{
    char lA[64];
    char lN[64];
    char lV[64];
    char lF[64];

    unsigned int               lFlags = 0;
    KMS::Modbus::RegisterValue lValue = 0;

    switch (sscanf_s(aV, "%[^;];%[^;];%[^;];%[^\n\r\t]", lA SizeInfo(lA), lN SizeInfo(lN), lV SizeInfo(lV), lF SizeInfo(lF)))
    {
    case 4: lFlags = KMS::Convert::ToUInt32(lF);
    case 3: lValue = ToRegisterValue(lV);
    case 2: break;

    default: KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid bit information", aV);
    }

    KMS::Modbus::Address lAddr = KMS::Convert::ToUInt16(lA);

    KMS::Modbus::Simulator::Item lItem(lN, lValue, lFlags);

    aMap->insert(KMS::Modbus::Simulator::ItemMap::value_type(lAddr, lItem));
}

KMS::Modbus::RegisterValue ToRegisterValue(const char* aIn)
{
    if (0 == _stricmp("false", aIn)) { return 0; }
    if (0 == _stricmp("true" , aIn)) { return 1; }

    if (0 == _stricmp("off", aIn)) { return KMS::Modbus::OFF; }
    if (0 == _stricmp("on" , aIn)) { return KMS::Modbus::ON ; }

    return KMS::Convert::ToUInt16(aIn);
}

void TraceKnown(const char* aOp, KMS::Modbus::Address aA, const KMS::Modbus::Simulator::Item& aItem, unsigned int aFlags)
{
    assert(NULL != aOp);

    if (0 != (aItem.mFlags & aFlags))
    {
        std::cout << aOp << " " << aItem.mName.c_str() << " (" << aA << ") = " << aItem.mValue << std::endl;
    }
}

void TraceUnknown(const char* aOp, KMS::Modbus::Address aA, KMS::Modbus::RegisterValue aV)
{
    assert(NULL != aOp);

    std::cout << KMS::Console::Color::RED;
    std::cout << aOp << " at " << aA << " = " << aV << std::endl;
    std::cout << KMS::Console::Color::WHITE;
}
