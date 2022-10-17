
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Modbus_Tool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/Convert.h>
#include <KMS/Installer.h>
#include <KMS/Modbus/Master_Com.h>

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

            try
            {
                KMS::Cfg::Configurator  lC;
                KMS::Installer          lInstaller;
                KMS::Modbus::Master_Com lM;
                KMS::Modbus::Tool       lT;

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lM);
                lC.AddConfigurable(&lT);

                lC.AddConfigurable(&Dbg::gLog);

                lT.InitMaster(&lM);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                Dbg::gLog.CloseLogFiles();

                lInstaller.Run();

                lResult = lT.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Tool::Tool() : mMaster(NULL)
        {
            mCoils           .SetCreator(DI::UInt<uint16_t>::Create);
            mDiscreteInputs  .SetCreator(DI::UInt<uint16_t>::Create);
            mHoldingRegisters.SetCreator(DI::UInt<uint16_t>::Create);
            mInputRegisters  .SetCreator(DI::UInt<uint16_t>::Create);
            mOperations      .SetCreator(DI::String::Create);

            AddEntry("Coils"           , &mCoils           , false, &MD_COILS);
            AddEntry("DiscreteInputs"  , &mDiscreteInputs  , false, &MD_DISCRETE_INPUTS);
            AddEntry("HoldingRegisters", &mHoldingRegisters, false, &MD_HOLDING_REGISTERS);
            AddEntry("InputRegisters"  , &mInputRegisters  , false, &MD_INPUT_REGISTERS);
            AddEntry("Operations"      , &mOperations      , false, &MD_OPERATIONS);
        }

        void Tool::InitMaster(Master* aMaster) { assert(NULL != aMaster); mMaster = aMaster; }

        void Tool::AddCoil           (const char* aN, uint16_t aA) { mCoils           .AddEntry(aN, new DI::UInt<uint16_t>(aA), true); }
        void Tool::AddDiscreteInput  (const char* aN, uint16_t aA) { mDiscreteInputs  .AddEntry(aN, new DI::UInt<uint16_t>(aA), true); }
        void Tool::AddHoldingRegister(const char* aN, uint16_t aA) { mHoldingRegisters.AddEntry(aN, new DI::UInt<uint16_t>(aA), true); }
        void Tool::AddInputRegister  (const char* aN, uint16_t aA) { mInputRegisters  .AddEntry(aN, new DI::UInt<uint16_t>(aA), true); }

        void Tool::AddOperation(const char* aO) { mOperations.AddEntry(new DI::String(aO), true); }

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

        void Tool::ExecuteOperation(const char* aO)
        {
            char lA[NAME_LENGTH];
            char lB[NAME_LENGTH];
            char lC[NAME_LENGTH];

            std::cout << "Operation: " << aO << std::endl;

            switch (sscanf_s(aO, "%[^ \n\r\t] %[^ \n\r\t] %[^ \n\r\t]", lA SizeInfo(lA), lB SizeInfo(lB), lC SizeInfo(lC)))
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

            default: KMS_EXCEPTION(MODBUS_CONFIG_INVALID, "Invalid operation format", aO);
            }

            KMS_EXCEPTION(MODBUS_CONFIG_INVALID, "Invalid operation name", aO);
        }

        int Tool::Run()
        {
            Connect();

            for (const DI::Object* lObj : mOperations.mInternal)
            {
                assert(NULL != lObj);

                const DI::String* lString = dynamic_cast<const DI::String*>(lObj);
                assert(NULL != lString);

                try
                {
                    ExecuteOperation(*lString);
                }
                KMS_CATCH
            }

            Disconnect();

            return 0;
        }

        // ===== Modbus functions ===========================================

        bool Tool::ReadCoil(const char* aName) { assert(NULL != mMaster); return mMaster->ReadCoil(ToAddress(mCoils, aName)); }

        bool Tool::ReadDiscreteInput(const char* aName) { assert(NULL != mMaster); return mMaster->ReadDiscreteInput(ToAddress(mDiscreteInputs, aName)); }

        uint16_t Tool::ReadHoldingRegister(const char* aName) { assert(NULL != mMaster); return mMaster->ReadHoldingRegister(ToAddress(mHoldingRegisters, aName)); }

        uint16_t Tool::ReadInputRegister(const char* aName) { assert(NULL != mMaster); return mMaster->ReadInputRegister(ToAddress(mInputRegisters, aName)); }

        void Tool::WriteSingleCoil(const char* aName, bool aValue) { assert(NULL != mMaster); mMaster->WriteSingleCoil(ToAddress(mCoils, aName), aValue); }

        void Tool::WriteSingleRegister(const char* aName, uint16_t aValue) { assert(NULL != mMaster); mMaster->WriteSingleRegister(ToAddress(mHoldingRegisters, aName), aValue); }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

uint16_t ToAddress(const KMS::DI::Dictionary& aMap, const char* aName)
{
    assert(NULL != aName);

    uint16_t lResult;

    const KMS::DI::Object* lObject = aMap.GetEntry_R(aName);
    if (NULL == lObject)
    {
        lResult = KMS::Convert::ToUInt16(aName);
    }
    else
    {
        const KMS::DI::UInt<uint16_t>* lAddr = dynamic_cast<const KMS::DI::UInt<uint16_t>*>(lObject);
        assert(NULL != lAddr);

        lResult = *lAddr;
    }

    return lResult;
}
