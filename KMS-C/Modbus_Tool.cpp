
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Modbus_Tool.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <winsock2.h>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Com/Port.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/Convert.h>
#include <KMS/Dbg/Stats.h>
#include <KMS/Dbg/Stats_Timer.h>
#include <KMS/Installer.h>
#include <KMS/Modbus/Master_CFG.h>
#include <KMS/Modbus/Master_IDevice.h>
#include <KMS/Modbus/Master_TCP.h>

#include <KMS/Modbus/Tool.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-ModbusTool.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_COILS            ("Coils[{Name}] = {Address}");
static const KMS::Cfg::MetaData MD_DISCRETE_INPUTS  ("DiscreteInputs[{Name}] = {Address}");
static const KMS::Cfg::MetaData MD_HOLDING_REGISTERS("HoldingRegisters[{Name}] = {Address}");
static const KMS::Cfg::MetaData MD_INPUT_REGISTERS  ("InputRegisters[{Name}] = {Address}");
static const KMS::Cfg::MetaData MD_OPERATIONS       ("Operations[{Name}] = {Address}");

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static uint16_t ToAddress(const KMS::DI::Dictionary& aMap, const char* aName);

namespace KMS
{

    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        int Tool::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);

            int lResult = __LINE__;

            Net::Thread_Startup();

            auto lET = new Dbg::Stats_Timer("Main_ExecutionTime");
            lET->Start();

            try
            {
                unsigned int           lArgStart = 1;
                Cfg::Configurator      lC;
                Installer              lInstaller;
                Com::Port              lPort;

                Modbus::Master_IDevice lMC(&lPort);
                Modbus::Master_TCP     lMT;
                Modbus::Tool           lT;

                // Be default, the tool runs in COM mode. This keep backward
                // compatibility.
                Modbus::Master* lM = &lMC;

                if (2 < aCount)
                {
                    if      (0 == strcmp("COM", aVector[1]))
                    {
                        lArgStart = 2;
                        lC.AddConfigurable(&lPort);
                    }
                    else if (0 == strcmp("TCP", aVector[1]))
                    {
                        lM        = &lMT;
                        lArgStart = 2;

                        lC.AddConfigurable(lMT.GetSocket());
                    }
                }

                Modbus::Master_Cfg lCfg(lM);

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lCfg);
                lC.AddConfigurable(&lT);

                lC.AddConfigurable(&Dbg::gLog);
                lC.AddConfigurable(&Dbg::gStats);

                lT.InitMaster(lM);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);
                lC.ParseArguments(aCount - lArgStart, aVector + lArgStart);

                lInstaller.Run();

                lResult = lT.Run();
            }
            KMS_CATCH_RESULT(lResult)

            lET->Stop();

            Net::Thread_Cleanup();

            return lResult;
        }

        Tool::Tool() : mMaster(NULL)
        {
            mCoils           .SetCreator(DI::UInt<uint16_t>::Create);
            mDiscreteInputs  .SetCreator(DI::UInt<uint16_t>::Create);
            mHoldingRegisters.SetCreator(DI::UInt<uint16_t>::Create);
            mInputRegisters  .SetCreator(DI::UInt<uint16_t>::Create);

            AddEntry("Coils"           , &mCoils           , false, &MD_COILS);
            AddEntry("DiscreteInputs"  , &mDiscreteInputs  , false, &MD_DISCRETE_INPUTS);
            AddEntry("HoldingRegisters", &mHoldingRegisters, false, &MD_HOLDING_REGISTERS);
            AddEntry("InputRegisters"  , &mInputRegisters  , false, &MD_INPUT_REGISTERS);
        }

        void Tool::InitMaster(Master* aMaster) { assert(NULL != aMaster); mMaster = aMaster; }

        void Tool::AddCoil           (const char* aN, uint16_t aA) { mCoils           .AddEntry(aN, new DI::UInt<uint16_t>(aA), true); }
        void Tool::AddDiscreteInput  (const char* aN, uint16_t aA) { mDiscreteInputs  .AddEntry(aN, new DI::UInt<uint16_t>(aA), true); }
        void Tool::AddHoldingRegister(const char* aN, uint16_t aA) { mHoldingRegisters.AddEntry(aN, new DI::UInt<uint16_t>(aA), true); }
        void Tool::AddInputRegister  (const char* aN, uint16_t aA) { mInputRegisters  .AddEntry(aN, new DI::UInt<uint16_t>(aA), true); }

        void Tool::Connect   () { assert(NULL != mMaster); mMaster->Connect   (); }
        void Tool::Disconnect() { assert(NULL != mMaster); mMaster->Disconnect(); }

        void Tool::Dump(FILE* aOut)
        {
            assert(NULL != aOut);

            assert(NULL != mMaster);

            const DI::UInt<uint16_t>* lAddr;

            fprintf(aOut, "Coils\n");
            for (const DI::Dictionary::Internal::value_type lVT : mCoils.mInternal)
            {
                assert(NULL != lVT.second);

                lAddr = dynamic_cast<const DI::UInt<uint16_t>*>(lVT.second.Get());
                assert(NULL != lAddr);

                fprintf(aOut, "    %s\t(%u)\t%s\n", lVT.first.c_str(), lAddr->Get(), mMaster->ReadCoil(*lAddr) ? "true" : "false");
            }

            fprintf(aOut, "Discrete inputs\n");
            for (const DI::Dictionary::Internal::value_type lVT : mDiscreteInputs.mInternal)
            {
                assert(NULL != lVT.second);

                lAddr = dynamic_cast<const DI::UInt<uint16_t>*>(lVT.second.Get());
                assert(NULL != lAddr);

                fprintf(aOut, "    %s\t(%u)\t%s\n", lVT.first.c_str(), lAddr->Get(), mMaster->ReadDiscreteInput(*lAddr) ? "true" : "false");
            }

            fprintf(aOut, "Holding registers\n");
            for (const DI::Dictionary::Internal::value_type lVT : mHoldingRegisters.mInternal)
            {
                assert(NULL != lVT.second);

                lAddr = dynamic_cast<const DI::UInt<uint16_t>*>(lVT.second.Get());
                assert(NULL != lAddr);

                fprintf(aOut, "    %s\t(%u)\t%u\n", lVT.first.c_str(), lAddr->Get(), mMaster->ReadHoldingRegister(*lAddr));
            }

            fprintf(aOut, "Input registers\n");
            for (const DI::Dictionary::Internal::value_type lVT : mInputRegisters.mInternal)
            {
                assert(NULL != lVT.second);

                lAddr = dynamic_cast<const DI::UInt<uint16_t>*>(lVT.second.Get());
                assert(NULL != lAddr);

                fprintf(aOut, "    %s\t(%u)\t%u\n", lVT.first.c_str(), lAddr->Get(), mMaster->ReadInputRegister(*lAddr));
            }
        }

        // ===== Modbus functions ===========================================

        bool Tool::ReadCoil(const char* aName) { assert(NULL != mMaster); return mMaster->ReadCoil(ToAddress(mCoils, aName)); }

        bool Tool::ReadDiscreteInput(const char* aName) { assert(NULL != mMaster); return mMaster->ReadDiscreteInput(ToAddress(mDiscreteInputs, aName)); }

        uint16_t Tool::ReadHoldingRegister(const char* aName) { assert(NULL != mMaster); return mMaster->ReadHoldingRegister(ToAddress(mHoldingRegisters, aName)); }

        uint16_t Tool::ReadInputRegister(const char* aName) { assert(NULL != mMaster); return mMaster->ReadInputRegister(ToAddress(mInputRegisters, aName)); }

        void Tool::WriteSingleCoil(const char* aName, bool aValue) { assert(NULL != mMaster); mMaster->WriteSingleCoil(ToAddress(mCoils, aName), aValue); }

        void Tool::WriteSingleRegister(const char* aName, uint16_t aValue) { assert(NULL != mMaster); mMaster->WriteSingleRegister(ToAddress(mHoldingRegisters, aName), aValue); }

        // ===== CLI::Tool ==================================================

        void Tool::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "Dump\n"
                "ReadCoil {AddrOrNAme}\n"
                "ReadDiscreteInput {AddrOrName}\n"
                "ReadHoldingRegister {AddrOrName}\n"
                "ReadInputRegister {AddrOrName}\n"
                "WriteSingleCoil {AddrOrName} {false|true}\n"
                "WriteSingleRegister {AddrOrName} {Value}\n");

            CLI::Tool::DisplayHelp(aOut);
        }

        void Tool::ExecuteCommand(const char* aC)
        {
            char lA[NAME_LENGTH];
            char lB[NAME_LENGTH];
            char lC[NAME_LENGTH];

            switch (sscanf_s(aC, "%[^ \n\r\t] %[^ \n\r\t] %[^ \n\r\t]", lA SizeInfo(lA), lB SizeInfo(lB), lC SizeInfo(lC)))
            {
            case 1:
                if (0 == strcmp("Dump", lA)) { Dump(stdout); return; }
                break;

            case 2:
                if (0 == strcmp("ReadCoil", lA))
                {
                    bool lRet = ReadCoil(lB);
                    std::cout << lRet << std::endl;
                    return;
                }
                if (0 == strcmp("ReadDiscreteInput", lA))
                {
                    bool lRet = ReadDiscreteInput(lB);
                    std::cout << lRet << std::endl;
                    return;
                }
                if (0 == strcmp("ReadHoldingRegister", lA))
                {
                    uint16_t lRet = ReadHoldingRegister(lB);
                    std::cout << lRet << std::endl;
                    return;
                }
                if (0 == strcmp("ReadInputRegister", lA))
                {
                    uint16_t lRet = ReadInputRegister(lB);
                    std::cout << lRet << std::endl;
                    return;
                }
                break;

            case 3:
                if (0 == strcmp("WriteSingleCoil"    , lA)) { WriteSingleCoil    (lB, Convert::ToBool  (lC)); return; }
                if (0 == strcmp("WriteSingleRegister", lA)) { WriteSingleRegister(lB, Convert::ToUInt16(lC)); return; }
            }

            CLI::Tool::ExecuteCommand(aC);
        }

        int Tool::Run()
        {
            Connect();

            auto lRet = CLI::Tool::Run();

            Disconnect();

            return lRet;
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

uint16_t ToAddress(const DI::Dictionary& aMap, const char* aName)
{
    assert(NULL != aName);

    uint16_t lResult;

    auto lObject = aMap.GetEntry_R(aName);
    if (NULL == lObject)
    {
        lResult = Convert::ToUInt16(aName);
    }
    else
    {
        auto lAddr = dynamic_cast<const DI::UInt<uint16_t>*>(lObject);
        assert(NULL != lAddr);

        lResult = *lAddr;
    }

    return lResult;
}
