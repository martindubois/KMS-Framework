
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DAQ_DigitalOutputModule.h

#include "Component.h"

// ===== Includes ===========================================================
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

        int DigitalOutputModule::ExecuteCommand(const char* aCmd)
        {
            assert(nullptr != aCmd);

            int lResult = UNKNOWN_COMMAND;

            char lCmd[LINE_LENGTH];
            char lValue[NAME_LENGTH];

            if      (0 == strcmp(aCmd, "DigitalOutput Display")) { lResult = Cmd_Display(); }
            else if (0 == strcmp(aCmd, "DigitalOutput List"   )) { lResult = Cmd_List(); }
            else if (1 == sscanf_s(aCmd, "DigitalOutput Set %s", lValue SizeInfo(lValue))) { lResult = Cmd_Set(lValue); }
            else if (1 == sscanf_s(aCmd, "DigitalInput %[^\n\r]", lCmd SizeInfo(lCmd))) { lResult = mDigitalOutputs.ExecuteCommand(lCmd); }

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

        int DigitalOutputModule::Cmd_Display()
        {
            DisplaySelected(std::cout);

            return 0;
        }

        int DigitalOutputModule::Cmd_List()
        {
            List(std::cout);

            std::cout << mDigitalOutputs.mInstances.size() << " digital outputs" << std::endl;

            return 0;
        }

        int DigitalOutputModule::Cmd_Set(const char* aValue)
        {
            auto lValue = Convert::ToBool(aValue);

            SetSelected(lValue);

            return 0;
        }

    }
}
