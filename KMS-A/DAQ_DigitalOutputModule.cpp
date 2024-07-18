
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DAQ_DigitalOutputModule.h

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>
#include <KMS/Convert.h>

#include <KMS/DAQ/DigitalOutputModule.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        DigitalOutputModule::DigitalOutputModule()
        {
            mDigitalOutputs.SetAllowedCmds(mDigitalOutputs.CMD_SELECT);
        }

        void DigitalOutputModule::DisplaySelected(std::ostream& aOut)
        {
            auto lDO = mDigitalOutputs.GetSelected();
            assert(nullptr != lDO);

            aOut << (lDO->Get() ? "true" : "false") << std::endl;
        }

        void DigitalOutputModule::List(std::ostream& aOut) const
        {
            for (auto lPair : mDigitalOutputs.mInstances)
            {
                assert(nullptr != lPair.second);

                aOut << lPair.first << "\t" << (reinterpret_cast<DigitalOutput*>(lPair.second)->Get() ? "true" : "false") << "\n";
            }

            aOut << std::endl;
        }

        void DigitalOutputModule::SetSelected(bool aValue)
        {
            auto lDO = mDigitalOutputs.GetSelected();
            assert(nullptr != lDO);

            lDO->Set(aValue);
        }

        // ===== CLI::ICommandParser ========================================

        int DigitalOutputModule::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if (0 == _stricmp(lCmd, "DigitalOutput")) { aCmd->Next(); lResult = Cmd(aCmd); }

            return lResult;
        }

        // ===== CLI::Module ================================================
        void DigitalOutputModule::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "DigitalOutput Display\n"
                "DigitalOutput List\n"
                "DigitalOutput Select {Name}\n"
                "DigitalOutput Set false|true\n");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int DigitalOutputModule::Cmd(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if      (0 == _stricmp(lCmd, "Display")) { aCmd->Next(); lResult = Cmd_Display(aCmd); }
            else if (0 == _stricmp(lCmd, "List"   )) { aCmd->Next(); lResult = Cmd_List   (aCmd); }
            else if (0 == _stricmp(lCmd, "Set"    )) { aCmd->Next(); lResult = Cmd_Set    (aCmd); }
            else
            {
                lResult = mDigitalOutputs.ExecuteCommand(aCmd);
            }

            return lResult;
        }

        int DigitalOutputModule::Cmd_Display(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            DisplaySelected(std::cout);

            return 0;
        }

        int DigitalOutputModule::Cmd_List(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            List(std::cout);

            std::cout << mDigitalOutputs.mInstances.size() << " digital outputs" << std::endl;

            return 0;
        }

        int DigitalOutputModule::Cmd_Set(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lValue = Convert::ToBool(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            SetSelected(lValue);

            return 0;
        }

    }
}
