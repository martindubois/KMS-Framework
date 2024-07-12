
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DAQ_AnalogInputModule.h

#include "Component.h"

// ===== Includes ===========================================================
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

        // ===== CLI::ICommandParser ========================================

        int AnalogInputModule::ExecuteCommand(const char* aCmd)
        {
            assert(nullptr != aCmd);

            int lResult = UNKNOWN_COMMAND;

            char lCmd[LINE_LENGTH];

            if      (0 == strcmp(aCmd, "AnalogInput Display")) { lResult = Cmd_Display(); }
            else if (0 == strcmp(aCmd, "AnalogInput List"   )) { lResult = Cmd_List   (); }
            else if (1 == sscanf_s(aCmd, "AnalogInput %[^\n\r]", lCmd SizeInfo(lCmd))) { lResult = mAnalogInputs.ExecuteCommand(lCmd); }

            return lResult;
        }

        // ===== CLI::Module ================================================
        void AnalogInputModule::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "AnalogInput Display\n"
                "AnalogInput List\n"
                "AnalogInput Select {Name}\n");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int AnalogInputModule::Cmd_Display()
        {
            DisplaySelected(std::cout);

            return 0;
        }

        int AnalogInputModule::Cmd_List()
        {
            List(std::cout);

            std::cout << mAnalogInputs.mInstances.size() << " analog inputs" << std::endl;

            return 0;
        }

    }
}
