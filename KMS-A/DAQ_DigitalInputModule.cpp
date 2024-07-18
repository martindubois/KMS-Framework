
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DAQ_DigitalInputModule.h

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>

#include <KMS/DAQ/DigitalInputModule.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        DigitalInputModule::DigitalInputModule()
        {
            mDigitalInputs.SetAllowedCmds(mDigitalInputs.CMD_SELECT);
        }

        void DigitalInputModule::DisplaySelected(std::ostream& aOut)
        {
            auto lDI = mDigitalInputs.GetSelected();
            assert(nullptr != lDI);

            aOut << (lDI->Read() ? "true" : "false") << std::endl;
        }

        void DigitalInputModule::List(std::ostream& aOut) const
        {
            for (auto lPair : mDigitalInputs.mInstances)
            {
                assert(nullptr != lPair.second);

                aOut << lPair.first << "\t" << (reinterpret_cast<DigitalInput*>(lPair.second)->Read() ? "true" : "false") << "\n";
            }

            aOut << std::endl;
        }

        // ===== CLI::ICommandParser ========================================

        int DigitalInputModule::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if (0 == _stricmp(lCmd, "DigitalInput")) { aCmd->Next(); lResult = Cmd(aCmd); }

            return lResult;
        }

        // ===== CLI::Module ================================================
        void DigitalInputModule::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "DigitalInput Display\n"
                "DigitalInput List\n"
                "DigitalInput Select {Name}\n");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int DigitalInputModule::Cmd(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if      (0 == _stricmp(lCmd, "Display")) { aCmd->Next(); lResult = Cmd_Display(aCmd); }
            else if (0 == _stricmp(lCmd, "List"   )) { aCmd->Next(); lResult = Cmd_List   (aCmd); }
            else
            {
                lResult = mDigitalInputs.ExecuteCommand(aCmd);
            }

            return lResult;
        }

        int DigitalInputModule::Cmd_Display(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            DisplaySelected(std::cout);

            return 0;
        }

        int DigitalInputModule::Cmd_List(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            List(std::cout);

            std::cout << mDigitalInputs.mInstances.size() << " digital inputs" << std::endl;

            return 0;
        }

    }
}
