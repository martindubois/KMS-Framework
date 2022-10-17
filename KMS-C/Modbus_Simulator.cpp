
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
#include <KMS/Cfg/MetaData.h>
#include <KMS/Console/Color.h>
#include <KMS/Convert.h>
#include <KMS/Installer.h>
#include <KMS/Modbus/Slave_Com.h>

#include <KMS/Modbus/Simulator.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-ModbusSim.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_COILS            ("Coils[{Address}] = {Name}[;{Value}][;{Flags}]");
static const KMS::Cfg::MetaData MD_DISCRETE_INPUTS  ("DiscreteInputs[{Address}] = {Name}[;{Value}][;{Flags}]");
static const KMS::Cfg::MetaData MD_HOLDING_REGISTERS("HoldingRegisters[{Address}] = {Name}[;{Value}][;{Flags}]");
static const KMS::Cfg::MetaData MD_INPUT_REGISTERS  ("InputRegisters[{Address}] = {Name}[;{Value}][;{Flags}]");

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

static KMS::DI::Object* CreateItem();

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

            int lResult = __LINE__;

            _crt_signal_t lHandler = NULL;

            try
            {
                KMS::Cfg::Configurator lC;
                KMS::Installer         lInstaller;
                Slave_Com lSl;
                Simulator lSi;

                lSi.InitSlave(&lSl);

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lSl);
                lC.AddConfigurable(&lSi);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                Dbg::gLog.CloseLogFiles();

                lInstaller.Run();

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

        Simulator::Simulator() : mSlave(NULL)
        {
            mCoils           .SetCreator(CreateItem);
            mDiscreteInputs  .SetCreator(CreateItem);
            mHoldingRegisters.SetCreator(CreateItem);
            mInputRegisters  .SetCreator(CreateItem);

            AddEntry("Coils"           , &mCoils           , false, &MD_COILS);
            AddEntry("DiscreteInputs"  , &mDiscreteInputs  , false, &MD_DISCRETE_INPUTS);
            AddEntry("HoldingRegisters", &mHoldingRegisters, false, &MD_HOLDING_REGISTERS);
            AddEntry("InputRegisters"  , &mInputRegisters  , false, &MD_INPUT_REGISTERS);
        }

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

        // Internal
        // //////////////////////////////////////////////////////////////////

        Simulator::Item::Item() : mFlags(0), mValue(0) {}

        // ===== DI::Value ==================================================

        unsigned int Simulator::Item::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            // TODO Validate aOutSize_byte

            return sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s;%u;%u", mName.c_str(), mValue, mFlags);
        }

        void Simulator::Item::Set(const char* aIn)
        {
            assert(NULL != aIn);

            char lN[64];
            char lV[64];
            char lF[64];

            unsigned int               lFlags = 0;
            KMS::Modbus::RegisterValue lValue = 0;

            switch (sscanf_s(aIn, "%[^;];%[^;];%[^\n\r\t]", lN SizeInfo(lN), lV SizeInfo(lV), lF SizeInfo(lF)))
            {
            case 3: lFlags = KMS::Convert::ToUInt32(lF);
            case 2: lValue = ToRegisterValue(lV);
            case 1: break;

            default: KMS_EXCEPTION(MODBUS_CONFIG_INVALID, "Invalid bit information", aIn);
            }

            mFlags = lFlags;
            mName  = lN;
            mValue = lValue;
        }

        // ===== DI::Object =================================================
        Simulator::Item::~Item() {}

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Message handlers ===========================================

        unsigned int Simulator::OnReadCoils(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            for (unsigned int i = 0; i < aData->mQty; i++)
            {
                bool lValue = false;

                const DI::Object* lObject = mCoils.GetEntry_R(aData->mStartAddr + i);
                if (NULL == lObject)
                {
                    TraceUnknown("Read Coil", aData->mStartAddr + i, OFF);
                }
                else
                {
                    const Item* lItem = dynamic_cast<const Item*>(lObject);
                    assert(NULL != lItem);

                    TraceKnown("Read Coil", aData->mStartAddr + i, *lItem, FLAG_VERBOSE_READ);

                    lValue = lItem->mValue;
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

                const DI::Object* lObject = mDiscreteInputs.GetEntry_R(aData->mStartAddr + i);
                if (NULL == lObject)
                {
                    TraceUnknown("Read Discrete Input", aData->mStartAddr + i, OFF);
                }
                else
                {
                    const Item* lItem = dynamic_cast<const Item*>(lObject);
                    assert(NULL != lItem);

                    TraceKnown("Read Discrete Input", aData->mStartAddr + i, *lItem, FLAG_VERBOSE_READ);

                    lValue = lItem->mValue;
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

                const DI::Object* lObject = mHoldingRegisters.GetEntry_R(aData->mStartAddr + i);
                if (NULL == lObject)
                {
                    TraceUnknown("Read Holding Register", aData->mStartAddr + i, 0);
                }
                else
                {
                    const Item* lItem = dynamic_cast<const Item*>(lObject);
                    assert(NULL != lItem);

                    TraceKnown("Read Holding Register", aData->mStartAddr + i, *lItem, FLAG_VERBOSE_READ);

                    lValue = lItem->mValue;
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

                const DI::Object* lObject = mInputRegisters.GetEntry_R(aData->mStartAddr + i);
                if (NULL == lObject)
                {
                    TraceUnknown("Read Input Register", aData->mStartAddr + i, 0);
                }
                else
                {
                    const Item* lItem = dynamic_cast<const Item*>(lObject);
                    assert(NULL != lItem);

                    TraceKnown("Read Input Register", aData->mStartAddr, *lItem, FLAG_VERBOSE_READ);

                    lValue = lItem->mValue;
                }

                WriteUInt16(aData->mBuffer, sizeof(RegisterValue) * i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnWriteSingleCoil(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            RegisterValue lValue = ReadUInt16(aData->mBuffer, 0);

            DI::Object* lObject = mCoils.GetEntry_RW(aData->mStartAddr);
            if (NULL == lObject)
            {
                TraceUnknown("Write Single Coil", aData->mStartAddr, lValue);
            }
            else
            {
                bool lBool = ON == lValue;
                unsigned int lFlags = FLAG_VERBOSE_WRITE;

                Item* lItem = dynamic_cast<Item*>(lObject);
                assert(NULL != lItem);

                if ((0 != lItem->mValue) != lBool)
                {
                    lItem->mValue = lBool;
                    lFlags |= FLAG_VERBOSE_CHANGE;
                }

                TraceKnown("Write Single Coil", aData->mStartAddr, *lItem, lFlags);
            }

            return 0;
        }

        unsigned int Simulator::OnWriteSingleRegister(Slave::MsgData* aData)
        {
            assert(NULL != aData);

            RegisterValue lValue = ReadUInt16(aData->mBuffer, 0);

            DI::Object* lObject = mHoldingRegisters.GetEntry_RW(aData->mStartAddr);
            if (NULL == lObject)
            {
                TraceUnknown("Write Single Register", aData->mStartAddr, lValue);
            }
            else
            {
                unsigned int lFlags = FLAG_VERBOSE_WRITE;

                Item* lItem = dynamic_cast<Item*>(lObject);
                assert(NULL != lItem);

                if (lItem->mValue != lValue)
                {
                    lItem->mValue = lValue;
                    lFlags |= FLAG_VERBOSE_CHANGE;
                }

                TraceKnown("Write Single Register", aData->mStartAddr, *lItem, lFlags);
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

KMS::DI::Object* CreateItem() { return new KMS::Modbus::Simulator::Item; }

KMS::Modbus::RegisterValue ToRegisterValue(const char* aIn)
{
    assert(NULL != aIn);
    
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
    std::cout << aOp << " at " << aA << " = " << aV;
    std::cout << KMS::Console::Color::WHITE << std::endl;
}
