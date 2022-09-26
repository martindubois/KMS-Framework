
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Modbus_Tool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt16.h>
#include <KMS/Convert.h>
#include <KMS/Modbus/Master_Com.h>

#include <KMS/Modbus/Tool.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-ModbusTool.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI::MetaData MD_COILS            ("Coils"           , "Coils[{Name}] = {Address}");
static const KMS::DI::MetaData MD_DISCRETE_INPUTS  ("DiscreteInputs"  , "DiscreteInputs[{Name}] = {Address}");
static const KMS::DI::MetaData MD_HOLDING_REGISTERS("HoldingRegisters", "HoldingRegisters[{Name}] = {Address}");
static const KMS::DI::MetaData MD_INPUT_REGISTERS  ("InputRegisters"  , "InputRegisters[{Name}] = {Address}");
static const KMS::DI::MetaData MD_OPERATIONS       ("Operations"      , "Operations[{Name}] = {Address}");

static const unsigned int FLAGS = KMS::DI::MetaData::FLAG_COPY_NAME | KMS::DI::MetaData::FLAG_DELETE_OBJECT | KMS::DI::MetaData::FLAG_DELETE_META_DATA;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static uint16_t ToAddress(const KMS::DI::Array& aMap, const char* aName);

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
                KMS::Modbus::Master_Com lM;
                KMS::Modbus::Tool       lT;

                lC.AddConfigurable(&lM);
                lC.AddConfigurable(&lT);

                lC.AddConfigurable(&Dbg::gLog);

                lT.InitMaster(&lM);

                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::HOME      ), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                Dbg::gLog.CloseLogFiles();

                lResult = lT.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Tool::Tool()
            : DI::Dictionary(NULL)
            , mCoils           (&MD_COILS)
            , mDiscreteInputs  (&MD_DISCRETE_INPUTS)
            , mHoldingRegisters(&MD_HOLDING_REGISTERS)
            , mInputRegisters  (&MD_INPUT_REGISTERS)
            , mOperations      (&MD_OPERATIONS)
            , mMaster(NULL)
        {
            mCoils           .SetCreator(DI::UInt16::Create);
            mDiscreteInputs  .SetCreator(DI::UInt16::Create);
            mHoldingRegisters.SetCreator(DI::UInt16::Create);
            mInputRegisters  .SetCreator(DI::UInt16::Create);
            mOperations      .SetCreator(DI::String::Create);

            AddEntry(&mCoils);
            AddEntry(&mDiscreteInputs);
            AddEntry(&mHoldingRegisters);
            AddEntry(&mInputRegisters);
            AddEntry(&mOperations);
        }

        void Tool::InitMaster(Master* aMaster) { assert(NULL != aMaster); mMaster = aMaster; }

        void Tool::AddCoil           (const char* aN, uint16_t aA) { mCoils           .AddEntry(new DI::UInt16(aA, new DI::MetaData(aN, NULL, FLAGS))); }
        void Tool::AddDiscreteInput  (const char* aN, uint16_t aA) { mDiscreteInputs  .AddEntry(new DI::UInt16(aA, new DI::MetaData(aN, NULL, FLAGS))); }
        void Tool::AddHoldingRegister(const char* aN, uint16_t aA) { mHoldingRegisters.AddEntry(new DI::UInt16(aA, new DI::MetaData(aN, NULL, FLAGS))); }
        void Tool::AddInputRegister  (const char* aN, uint16_t aA) { mInputRegisters  .AddEntry(new DI::UInt16(aA, new DI::MetaData(aN, NULL, FLAGS))); }

        void Tool::AddOperation(const char* aO) { mOperations.AddEntry(new DI::String(aO, &DI::META_DATA_DELETE_OBJECT)); }

        void Tool::Connect   () { assert(NULL != mMaster); mMaster->Connect   (); }
        void Tool::Disconnect() { assert(NULL != mMaster); mMaster->Disconnect(); }

        void Tool::Dump(FILE* aOut)
        {
            assert(NULL != aOut);

            assert(NULL != mMaster);

            const DI::UInt16* lAddr;

            fprintf(aOut, "Coils\n");
            const DI::Array::Internal& lInternalC = mCoils.GetInternal();
            for (const DI::Object* lObj : lInternalC)
            {
                assert(NULL != lObj);

                lAddr = dynamic_cast<const DI::UInt16*>(lObj);
                assert(NULL != lAddr);

                fprintf(aOut, "    %s\t(%u)\t%s\n", lAddr->GetName(), lAddr->Get(), mMaster->ReadCoil(*lAddr) ? "true" : "false");
            }

            fprintf(aOut, "Discrete inputs\n");
            const DI::Array::Internal& lInternalDI = mDiscreteInputs.GetInternal();
            for (const DI::Object* lObj : lInternalDI)
            {
                assert(NULL != lObj);

                lAddr = dynamic_cast<const DI::UInt16*>(lObj);
                assert(NULL != lAddr);

                fprintf(aOut, "    %s\t(%u)\t%s\n", lAddr->GetName(), lAddr->Get(), mMaster->ReadDiscreteInput(*lAddr) ? "true" : "false");
            }

            fprintf(aOut, "Holding registers\n");
            const DI::Array::Internal& lInternalHR = mHoldingRegisters.GetInternal();
            for (const DI::Object* lObj : lInternalHR)
            {
                assert(NULL != lObj);

                lAddr = dynamic_cast<const DI::UInt16*>(lObj);
                assert(NULL != lAddr);

                fprintf(aOut, "    %s\t(%u)\t%u\n", lAddr->GetName(), lAddr->Get(), mMaster->ReadHoldingRegister(*lAddr));
            }

            fprintf(aOut, "Input registers\n");
            const DI::Array::Internal& lInternalIR = mInputRegisters.GetInternal();
            for (const DI::Object* lObj : lInternalIR)
            {
                assert(NULL != lObj);

                lAddr = dynamic_cast<const DI::UInt16*>(lObj);
                assert(NULL != lAddr);

                fprintf(aOut, "    %s\t(%u)\t%u\n", lAddr->GetName(), lAddr->Get(), mMaster->ReadInputRegister(*lAddr));
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

            default: KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid operation format", aO);
            }

            KMS_EXCEPTION_WITH_INFO(CONFIG_VALUE, "Invalid operation name", aO);
        }

        int Tool::Run()
        {
            Connect();

            const DI::Array::Internal& lInternal = mOperations.GetInternal();
            for (const DI::Object* lObj : lInternal)
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

uint16_t ToAddress(const KMS::DI::Array& aMap, const char* aName)
{
    assert(NULL != aName);

    uint16_t lResult;

    const KMS::DI::Object* lObject = aMap[aName];
    if (NULL == lObject)
    {
        lResult = KMS::Convert::ToUInt16(aName);
    }
    else
    {
        const KMS::DI::UInt16* lAddr = dynamic_cast<const KMS::DI::UInt16*>(lObject);
        assert(NULL != lAddr);

        lResult = *lAddr;
    }

    return lResult;
}
