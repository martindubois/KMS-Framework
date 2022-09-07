
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Modbus_Tool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Convert.h>
#include <KMS/Modbus/Master_Com.h>

#include <KMS/Modbus/Tool.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-ModbusTool.cfg")

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static uint16_t ToAddress(const KMS::Modbus::Tool::AddressMap& aMap, const char* aName);

static uint16_t ToNameAndAddress(const char* aIn, char* aName, unsigned int aNameSize_byte);

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
            assert(NULL != aVector[0]);

            int lResult = __LINE__;

            try
            {
                KMS::Cfg::Configurator  lC;
                KMS::Modbus::Master_Com lM;
                KMS::Modbus::Tool       lT;

                lM.InitConfigurator(&lC);
                lT.InitConfigurator(&lC);

                Dbg::gLog.InitConfigurator(&lC);

                lT.InitMaster(&lM);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::HOME      ), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lResult = lT.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Tool::Tool() : mMaster(NULL) {}

        void Tool::InitMaster(Master* aMaster) { assert(NULL != aMaster); mMaster = aMaster; }

        void Tool::AddCoil           (const char* aN, uint16_t aA) { assert(NULL != aN); mCoils           .insert(AddressMap::value_type(aN, aA)); }
        void Tool::AddDiscreteInput  (const char* aN, uint16_t aA) { assert(NULL != aN); mDiscreteInputs  .insert(AddressMap::value_type(aN, aA)); }
        void Tool::AddHoldingRegister(const char* aN, uint16_t aA) { assert(NULL != aN); mHoldingRegisters.insert(AddressMap::value_type(aN, aA)); }
        void Tool::AddInputRegister  (const char* aN, uint16_t aA) { assert(NULL != aN); mInputRegisters  .insert(AddressMap::value_type(aN, aA)); }

        void Tool::AddOperation(const char* aO) { assert(NULL != aO); mOperations.push_back(aO); }

        void Tool::Connect   () { assert(NULL != mMaster); mMaster->Connect   (); }
        void Tool::Disconnect() { assert(NULL != mMaster); mMaster->Disconnect(); }

        void Tool::Dump(FILE* aOut)
        {
            assert(NULL != aOut);

            assert(NULL != mMaster);

            fprintf(aOut, "Coils\n");
            for (AddressMap::value_type& lP : mCoils)
            {
                fprintf(aOut, "    %s\t(%u)\t%s\n", lP.first.c_str(), lP.second, mMaster->ReadCoil(lP.second) ? "true" : "false");
            }

            fprintf(aOut, "Discrete inputs\n");
            for (AddressMap::value_type& lP : mDiscreteInputs)
            {
                fprintf(aOut, "    %s\t(%u)\t%s\n", lP.first.c_str(), lP.second, mMaster->ReadDiscreteInput(lP.second) ? "true" : "false");
            }

            fprintf(aOut, "Holding registers\n");
            for (AddressMap::value_type& lP : mHoldingRegisters)
            {
                fprintf(aOut, "    %s\t(%u)\t%u\n", lP.first.c_str(), lP.second, mMaster->ReadHoldingRegister(lP.second));
            }

            fprintf(aOut, "Input registers\n");
            for (AddressMap::value_type& lP : mInputRegisters)
            {
                fprintf(aOut, "    %s\t(%u)\t%u\n", lP.first.c_str(), lP.second, mMaster->ReadInputRegister(lP.second));
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

            for (const std::string& lO : mOperations)
            {
                try
                {
                    ExecuteOperation(lO.c_str());
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

        // ===== Cfg::Configurable ==========================================

        bool Tool::AddAttribute(const char* aA, const char* aV)
        {
            char lName[NAME_LENGTH];

            CFG_IF("Coils"           ) { AddCoil           (lName, ToNameAndAddress(aV, lName, sizeof(lName))); return true; }
            CFG_IF("DiscreteInputs"  ) { AddDiscreteInput  (lName, ToNameAndAddress(aV, lName, sizeof(lName))); return true; }
            CFG_IF("HoldingRegisters") { AddHoldingRegister(lName, ToNameAndAddress(aV, lName, sizeof(lName))); return true; }
            CFG_IF("InputRegisters"  ) { AddInputRegister  (lName, ToNameAndAddress(aV, lName, sizeof(lName))); return true; }

            CFG_CALL("Operations", AddOperation);

            return Cfg::Configurable::AddAttribute(aA, aV);
        }

        void Tool::DisplayHelp(FILE* aOut) const
        {
            fprintf(aOut,
                "===== KMS::Modbus::Tool =====\n"
                "Coils += {Name}:{Addr}\n"
                "    Associate a name to and address\n"
                "DiscreteInputs += {Name}:{Addr}\n"
                "    Associate a name to and address\n"
                "HoldingRegisters += {Name}:{Addr}\n"
                "    Associate a name to and address\n"
                "InputRegisters += {Name}:{Addr}\n"
                "    Associate a name to and address\n"
                "Operations += {Operation} {Args...}\n"
                "    Operation to execute\n");

            Cfg::Configurable::DisplayHelp(aOut);
        }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

uint16_t ToAddress(const KMS::Modbus::Tool::AddressMap& aMap, const char* aName)
{
    assert(NULL != aName);

    uint16_t lResult;

    KMS::Modbus::Tool::AddressMap::const_iterator lIt = aMap.find(aName);
    if (aMap.end() == lIt)
    {
        lResult = KMS::Convert::ToUInt16(aName);
    }
    else
    {
        lResult = lIt->second;
    }

    return lResult;
}

uint16_t ToNameAndAddress(const char* aIn, char* aName, unsigned int aNameSize_byte)
{
    assert(NULL != aIn);
    assert(NULL != aName);
    assert(0 < aNameSize_byte);

    unsigned int lResult;

    if (   (2 != sscanf_s(aIn, "%[^;];0x%x", aName SizeInfoV(aNameSize_byte), &lResult))
        && (2 != sscanf_s(aIn, "%[^;];%u"  , aName SizeInfoV(aNameSize_byte), &lResult)))
    {
        KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid Modbus name and address pair", aIn);
    }

    if (0xffff < lResult)
    {
        KMS_EXCEPTION_WITH_INFO(CONFIG_VALUE, "Invalid Modbus address", lResult);
    }

    return lResult;
}
