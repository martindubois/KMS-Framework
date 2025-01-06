
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DAQ_AnalogOutputModule.h

#include "Component.h"

// ===== C++ ================================================================
#include <regex>

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>
#include <KMS/Convert.h>

#include <KMS/DAQ/AnalogOutputModule.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        AnalogOutputModule::AnalogOutputModule()
        {
            mAnalogOutputs.SetAllowedCmds(mAnalogOutputs.CMD_SELECT);
        }

        void AnalogOutputModule::DisplaySelected(std::ostream& aOut)
        {
            auto lAO = mAnalogOutputs.GetSelected();
            assert(nullptr != lAO);

            aOut << lAO->Get() << std::endl;
        }

        void AnalogOutputModule::List(std::ostream& aOut) const
        {
            for (auto lPair : mAnalogOutputs.mInstances)
            {
                assert(nullptr != lPair.second);

                aOut << lPair.first << "\t" << reinterpret_cast<AnalogOutput*>(lPair.second)->Get() << "\n";
            }

            aOut << std::endl;
        }

        unsigned int AnalogOutputModule::List(std::ostream& aOut, const char* aRegEx) const
        {
            assert(nullptr != aRegEx);

            unsigned int lResult = 0;

            std::regex lRegEx(aRegEx);

            for (auto lPair : mAnalogOutputs.mInstances)
            {
                assert(nullptr != lPair.second);

                if (std::regex_match(lPair.first, lRegEx))
                {
                    aOut << lPair.first << "\t" << reinterpret_cast<AnalogOutput*>(lPair.second)->Get() << "\n";

                    lResult++;
                }
            }

            aOut << std::endl;

            return lResult;
        }

        void AnalogOutputModule::WriteSelected(double aValue)
        {
            auto lAO = mAnalogOutputs.GetSelected();
            assert(nullptr != lAO);

            lAO->Write(aValue);
        }

        // ===== CLI::ICommandParser ========================================

        int AnalogOutputModule::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lResult = UNKNOWN_COMMAND;

            auto lCmd = aCmd->GetCurrent();

            if (0 == _stricmp(lCmd, "AnalogOutput")) { aCmd->Next(); lResult = Cmd(aCmd); }

            return lResult;
        }

        // ===== CLI::Module ================================================
        void AnalogOutputModule::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "AnalogOutput Display\n"
                "AnalogOutput List [RegEx]\n"
                "AnalogOutput Select {Name}\n"
                "AnalogOutput Write {Value}\n");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int AnalogOutputModule::Cmd(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if      (0 == _stricmp(lCmd, "Display")) { aCmd->Next(); lResult = Cmd_Display(aCmd); }
            else if (0 == _stricmp(lCmd, "List"   )) { aCmd->Next(); lResult = Cmd_List   (aCmd); }
            else if (0 == _stricmp(lCmd, "Write"  )) { aCmd->Next(); lResult = Cmd_Write  (aCmd); }
            else
            {
                lResult = mAnalogOutputs.ExecuteCommand(aCmd);
            }

            return lResult;
        }

        int AnalogOutputModule::Cmd_Display(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            DisplaySelected(std::cout);

            return 0;
        }

        int AnalogOutputModule::Cmd_List(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            if (aCmd->IsAtEnd())
            {
                List(std::cout);
            }
            else
            {
                char lText[LINE_LENGTH];

                aCmd->GetRemaining(lText, sizeof(lText));

                auto lCount = List(std::cout, lText);

                std::cout << lCount << " of ";
            }

            std::cout << mAnalogOutputs.mInstances.size() << " analog outputs" << std::endl;

            return 0;
        }

        int AnalogOutputModule::Cmd_Write(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lValue = Convert::ToDouble(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            WriteSelected(lValue);

            return 0;
        }

    }
}
