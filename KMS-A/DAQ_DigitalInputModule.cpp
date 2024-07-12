
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DAQ_DigitalInputModule.h

#include "Component.h"

// ===== Includes ===========================================================
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

        int DigitalInputModule::ExecuteCommand(const char* aCmd)
        {
            assert(nullptr != aCmd);

            int lResult = UNKNOWN_COMMAND;

            char lCmd[LINE_LENGTH];

            if      (0 == strcmp(aCmd, "DigitalInput Display" )) { lResult = Cmd_Display(); }
            else if (0 == strcmp(aCmd, "DigitalInput List"    )) { lResult = Cmd_List(); }
            else if (1 == sscanf_s(aCmd, "DigitalInput %[^\n\r]", lCmd SizeInfo(lCmd))) { lResult = mDigitalInputs.ExecuteCommand(lCmd); }

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

        int DigitalInputModule::Cmd_Display()
        {
            DisplaySelected(std::cout);

            return 0;
        }

        int DigitalInputModule::Cmd_List()
        {
            List(std::cout);

            std::cout << mDigitalInputs.mInstances.size() << " digital inputs" << std::endl;

            return 0;
        }

    }
}
