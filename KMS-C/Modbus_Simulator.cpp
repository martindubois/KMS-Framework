
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Simulator.cpp

#include "Component.h"

// ===== C ==================================================================
#include <signal.h>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Com/Port.h>
#include <KMS/Console/Color.h>
#include <KMS/Convert.h>
#include <KMS/Main.h>
#include <KMS/Modbus/Slave_Cfg.h>
#include <KMS/Modbus/Slave_IDevice.h>

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

// Static variable
// //////////////////////////////////////////////////////////////////////////

static KMS::Modbus::Simulator* sSimulator = nullptr;

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
            _crt_signal_t lHandler = nullptr;

            KMS_MAIN_BEGIN;
            {
                Com::Port     lPort;
                Slave_IDevice lSl(&lPort);
                Simulator     lSi;

                Modbus::Slave_Cfg lCfg(&lSl);

                lSi.InitSlave(&lSl);

                lConfigurator.AddConfigurable(&lCfg);
                lConfigurator.AddConfigurable(&lPort);
                lConfigurator.AddConfigurable(&lSi);

                lConfigurator.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lConfigurator.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lConfigurator.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);

                KMS_MAIN_PARSE_ARGS(aCount, aVector);

                KMS_MAIN_VALIDATE;

                sSimulator = &lSi;

                lHandler = signal(SIGINT, OnCtrlC);

                std::cout << "Press Ctrl-C to stop" << std::endl;

                lResult = lSi.Run();
            }
            KMS_MAIN_END;

            signal(SIGINT, lHandler);

            sSimulator = nullptr;

            KMS_MAIN_RETURN;
        }

        Simulator::Simulator()
            : mSlave(nullptr)
            // ===== Callbacks ==============================================
            , ON_READ_COILS            (this, &Simulator::OnReadCoils)
            , ON_READ_DISCRETE_INPUTS  (this, &Simulator::OnReadDiscreteInputs)
            , ON_READ_HOLDING_REGISTERS(this, &Simulator::OnReadHoldingRegisters)
            , ON_READ_INPUT_REGISTERS  (this, &Simulator::OnReadInputRegisters)
            , ON_WRITE_SINGLE_COIL     (this, &Simulator::OnWriteSingleCoil)
            , ON_WRITE_SINGLE_REGISTER (this, &Simulator::OnWriteSingleRegister)
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
            assert(nullptr != aSlave);

            mSlave = aSlave;

            mSlave->mOnReadCoils            = &ON_READ_COILS;
            mSlave->mOnReadDiscreteInputs   = &ON_READ_DISCRETE_INPUTS;
            mSlave->mOnReadHoldingRegisters = &ON_READ_HOLDING_REGISTERS;
            mSlave->mOnReadInputRegisters   = &ON_READ_INPUT_REGISTERS;
            mSlave->mOnWriteSingleCoil      = &ON_WRITE_SINGLE_COIL;
            mSlave->mOnWriteSingleRegister  = &ON_WRITE_SINGLE_REGISTER;
        }

        int Simulator::Run()
        {
            assert(nullptr != mSlave);

            if (!mSlave->Connect())
            {
                return __LINE__;
            }

            mSlave->Run();

            return 0;
        }

        void Simulator::Stop() { assert(nullptr != mSlave); mSlave->Stop(); }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Simulator::Item::Item() : mFlags(0), mValue(0) {}

        // ===== DI::Value ==================================================

        unsigned int Simulator::Item::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);

            // TODO Validate aOutSize_byte

            return sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s;%u;%u", mName.c_str(), mValue, mFlags);
        }

        void Simulator::Item::Set(const char* aIn)
        {
            assert(nullptr != aIn);

            char lN[64];
            char lV[64];
            char lF[64];

            unsigned int          lFlags = 0;
            Modbus::RegisterValue lValue = 0;

            switch (sscanf_s(aIn, "%[^;];%[^;];%[^\n\r\t]", lN SizeInfo(lN), lV SizeInfo(lV), lF SizeInfo(lF)))
            {
            case 3: lFlags = Convert::ToUInt32(lF);
            case 2: lValue = ToRegisterValue(lV);
            case 1: break;

            default: KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid bit information", aIn);
            }

            mFlags = lFlags;
            mName  = lN;
            mValue = lValue;
        }

        bool Simulator::Item::Set_Try(const char* aIn)
        {
            assert(nullptr != aIn);

            char lN[64];
            char lV[64];
            char lF[64];

            unsigned int          lFlags = 0;
            Modbus::RegisterValue lValue = 0;

            switch (sscanf_s(aIn, "%[^;];%[^;];%[^\n\r\t]", lN SizeInfo(lN), lV SizeInfo(lV), lF SizeInfo(lF)))
            {
            case 3: lFlags = Convert::ToUInt32(lF);
            case 2: lValue = ToRegisterValue(lV);
            case 1: break;

            default: return false;
            }

            mFlags = lFlags;
            mName = lN;
            mValue = lValue;

            return true;
        }

        // ===== DI::Object =================================================

        Simulator::Item::~Item() {}

        bool Simulator::Item::Clear()
        {
            auto lResult = !mName.empty();

            mFlags = 0;
            mName  = "";
            mValue = 0;

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Callbacks ==================================================

        unsigned int Simulator::OnReadCoils(void*, void* aData)
        {
            assert(nullptr != aData);

            auto lData = reinterpret_cast<Slave::MsgData*>(aData);

            for (unsigned int i = 0; i < lData->mQty; i++)
            {
                auto lValue = false;

                auto lObject = mCoils.GetEntry_R(lData->mStartAddr + i);
                if (nullptr == lObject)
                {
                    TraceUnknown("Read Coil", lData->mStartAddr + i, OFF);
                }
                else
                {
                    auto lItem = dynamic_cast<const Item*>(lObject);
                    assert(nullptr != lItem);

                    TraceKnown("Read Coil", lData->mStartAddr + i, *lItem, FLAG_VERBOSE_READ);

                    lValue = lItem->mValue;
                }

                WriteBit(lData->mBuffer, 0, i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnReadDiscreteInputs(void*, void* aData)
        {
            assert(nullptr != aData);

            auto lData = reinterpret_cast<Slave::MsgData*>(aData);

            for (unsigned int i = 0; i < lData->mQty; i++)
            {
                auto lValue = false;

                auto lObject = mDiscreteInputs.GetEntry_R(lData->mStartAddr + i);
                if (nullptr == lObject)
                {
                    TraceUnknown("Read Discrete Input", lData->mStartAddr + i, OFF);
                }
                else
                {
                    auto lItem = dynamic_cast<const Item*>(lObject);
                    assert(nullptr != lItem);

                    TraceKnown("Read Discrete Input", lData->mStartAddr + i, *lItem, FLAG_VERBOSE_READ);

                    lValue = lItem->mValue;
                }

                WriteBit(lData->mBuffer, 0, i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnReadHoldingRegisters(void*, void* aData)
        {
            assert(nullptr != aData);

            auto lData = reinterpret_cast<Slave::MsgData*>(aData);

            for (unsigned int i = 0; i < lData->mQty; i++)
            {
                RegisterValue lValue = 0;

                auto lObject = mHoldingRegisters.GetEntry_R(lData->mStartAddr + i);
                if (nullptr == lObject)
                {
                    TraceUnknown("Read Holding Register", lData->mStartAddr + i, 0);
                }
                else
                {
                    auto lItem = dynamic_cast<const Item*>(lObject);
                    assert(nullptr != lItem);

                    TraceKnown("Read Holding Register", lData->mStartAddr + i, *lItem, FLAG_VERBOSE_READ);

                    lValue = lItem->mValue;
                }

                WriteUInt16(lData->mBuffer, sizeof(RegisterValue) * i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnReadInputRegisters(void*, void* aData)
        {
            assert(nullptr != aData);

            auto lData = reinterpret_cast<Slave::MsgData*>(aData);

            for (unsigned int i = 0; i < lData->mQty; i++)
            {
                RegisterValue lValue = 0;

                auto lObject = mInputRegisters.GetEntry_R(lData->mStartAddr + i);
                if (nullptr == lObject)
                {
                    TraceUnknown("Read Input Register", lData->mStartAddr + i, 0);
                }
                else
                {
                    auto lItem = dynamic_cast<const Item*>(lObject);
                    assert(nullptr != lItem);

                    TraceKnown("Read Input Register", lData->mStartAddr, *lItem, FLAG_VERBOSE_READ);

                    lValue = lItem->mValue;
                }

                WriteUInt16(lData->mBuffer, sizeof(RegisterValue) * i, lValue);
            }

            return 0;
        }

        unsigned int Simulator::OnWriteSingleCoil(void*, void* aData)
        {
            assert(nullptr != aData);

            auto lData = reinterpret_cast<Slave::MsgData*>(aData);

            auto lValue = ReadUInt16(lData->mBuffer, 0);

            auto lObject = mCoils.GetEntry_RW(lData->mStartAddr);
            if (nullptr == lObject)
            {
                TraceUnknown("Write Single Coil", lData->mStartAddr, lValue);
            }
            else
            {
                auto lBool = ON == lValue;
                unsigned int lFlags = FLAG_VERBOSE_WRITE;

                auto lItem = dynamic_cast<Item*>(lObject);
                assert(nullptr != lItem);

                if ((0 != lItem->mValue) != lBool)
                {
                    lItem->mValue = lBool;
                    lFlags |= FLAG_VERBOSE_CHANGE;
                }

                TraceKnown("Write Single Coil", lData->mStartAddr, *lItem, lFlags);
            }

            return 0;
        }

        unsigned int Simulator::OnWriteSingleRegister(void*, void* aData)
        {
            assert(nullptr != aData);

            auto lData = reinterpret_cast<Slave::MsgData*>(aData);

            auto lValue = ReadUInt16(lData->mBuffer, 0);

            auto lObject = mHoldingRegisters.GetEntry_RW(lData->mStartAddr);
            if (nullptr == lObject)
            {
                TraceUnknown("Write Single Register", lData->mStartAddr, lValue);
            }
            else
            {
                unsigned int lFlags = FLAG_VERBOSE_WRITE;

                auto lItem = dynamic_cast<Item*>(lObject);
                assert(nullptr != lItem);

                if (lItem->mValue != lValue)
                {
                    lItem->mValue = lValue;
                    lFlags |= FLAG_VERBOSE_CHANGE;
                }

                TraceKnown("Write Single Register", lData->mStartAddr, *lItem, lFlags);
            }

            return 0;
        }

    }
}

using namespace KMS;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void OnCtrlC(int aSignal)
{
    assert(SIGINT == aSignal);

    assert(nullptr != sSimulator);

    std::cout << "Ctrl-C" << std::endl;

    sSimulator->Stop();
}

DI::Object* CreateItem() { return new Modbus::Simulator::Item; }

Modbus::RegisterValue ToRegisterValue(const char* aIn)
{
    assert(nullptr != aIn);
    
    if (0 == _stricmp("false", aIn)) { return 0; }
    if (0 == _stricmp("true" , aIn)) { return 1; }

    if (0 == _stricmp("off", aIn)) { return Modbus::OFF; }
    if (0 == _stricmp("on" , aIn)) { return Modbus::ON ; }

    return Convert::ToUInt16(aIn);
}

void TraceKnown(const char* aOp, Modbus::Address aA, const Modbus::Simulator::Item& aItem, unsigned int aFlags)
{
    assert(nullptr != aOp);

    if (0 != (aItem.mFlags & aFlags))
    {
        std::cout << aOp << " " << aItem.mName.c_str() << " (" << aA << ") = " << aItem.mValue << std::endl;
    }
}

void TraceUnknown(const char* aOp, Modbus::Address aA, Modbus::RegisterValue aV)
{
    assert(nullptr != aOp);

    std::cout << Console::Color::RED;
    std::cout << aOp << " at " << aA << " = " << aV;
    std::cout << Console::Color::WHITE << std::endl;
}
