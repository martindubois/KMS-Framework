
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DAQ_AnalogInputModule.h

#include "Component.h"

// ===== C++ ================================================================
#include <regex>

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>

#include <KMS/DAQ/AnalogInputModule.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        AnalogInputModule::AnalogInputModule()
        {
            mAnalogInputs.SetAllowedCmds(mAnalogInputs.CMD_SELECT);
        }

        void AnalogInputModule::DisplaySelected(std::ostream& aOut)
        {
            auto lAI = mAnalogInputs.GetSelected();
            assert(nullptr != lAI);

            aOut << lAI->Read() << std::endl;
        }

        void AnalogInputModule::List(std::ostream& aOut) const
        {
            for (auto lPair : mAnalogInputs.mInstances)
            {
                assert(nullptr != lPair.second);

                aOut << lPair.first << "\t" << reinterpret_cast<AnalogInput*>(lPair.second)->Read() << "\n";
            }

            aOut << std::endl;
        }

        unsigned int AnalogInputModule::List(std::ostream& aOut, const char* aRegEx) const
        {
            assert(nullptr != aRegEx);

            unsigned int lResult = 0;

            std::regex lRegEx(aRegEx);

            for (auto lPair : mAnalogInputs.mInstances)
            {
                assert(nullptr != lPair.second);

                if (std::regex_match(lPair.first, lRegEx))
                {
                    aOut << lPair.first << "\t" << reinterpret_cast<AnalogInput*>(lPair.second)->Read() << "\n";

                    lResult++;
                }
            }

            aOut << std::endl;

            return lResult;
        }

        // ===== CLI::ICommandParser ========================================

        int AnalogInputModule::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if (0 == _stricmp(lCmd, "AnalogInput")) { aCmd->Next(); lResult = Cmd(aCmd); }

            return lResult;
        }

        // ===== CLI::Module ================================================
        void AnalogInputModule::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "AnalogInput Display\n"
                "AnalogInput List [RegEx]\n"
                "AnalogInput Select {Name}\n");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int AnalogInputModule::Cmd(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if      (0 == _stricmp(lCmd, "Display")) { aCmd->Next(); lResult = Cmd_Display(aCmd); }
            else if (0 == _stricmp(lCmd, "List"   )) { aCmd->Next(); lResult = Cmd_List   (aCmd); }
            else
            {
                lResult = mAnalogInputs.ExecuteCommand(aCmd);
            }

            return lResult;
        }

        int AnalogInputModule::Cmd_Display(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            DisplaySelected(std::cout);

            return 0;
        }

        int AnalogInputModule::Cmd_List(CLI::CommandLine* aCmd)
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

            std::cout << mAnalogInputs.mInstances.size() << " analog inputs" << std::endl;

            return 0;
        }

    }
}
