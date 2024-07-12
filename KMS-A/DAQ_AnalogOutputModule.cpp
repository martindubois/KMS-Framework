
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DAQ_AnalogOutputModule.h

#include "Component.h"

// ===== Includes ===========================================================
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

        void AnalogOutputModule::WriteSelected(double aValue)
        {
            auto lAO = mAnalogOutputs.GetSelected();
            assert(nullptr != lAO);

            lAO->Write(aValue);
        }

        // ===== CLI::ICommandParser ========================================

        int AnalogOutputModule::ExecuteCommand(const char* aCmd)
        {
            assert(nullptr != aCmd);

            int lResult = UNKNOWN_COMMAND;

            char   lCmd[LINE_LENGTH];
            double lValue;

            if      (0 == strcmp(aCmd, "AnalogOutput Display")) { lResult = Cmd_Display(); }
            else if (0 == strcmp(aCmd, "AnalogOutput List"   )) { lResult = Cmd_List   (); }
            else if (1 == sscanf_s(aCmd, "AnalogOutput Write %lf", &lValue)) { lResult = Cmd_Write(lValue); }
            else if (1 == sscanf_s(aCmd, "AnalogOutput %[^\n\r]", lCmd SizeInfo(lCmd))) { lResult = mAnalogOutputs.ExecuteCommand(lCmd); }

            return lResult;
        }

        // ===== CLI::Module ================================================
        void AnalogOutputModule::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "AnalogOutput Display\n"
                "AnalogOutput List\n"
                "AnalogOutput Select {Name}\n"
                "AnalogOutput Write {Value}\n");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int AnalogOutputModule::Cmd_Display()
        {
            DisplaySelected(std::cout);

            return 0;
        }

        int AnalogOutputModule::Cmd_List()
        {
            List(std::cout);

            std::cout << mAnalogOutputs.mInstances.size() << " analog outputs" << std::endl;

            return 0;
        }

        int AnalogOutputModule::Cmd_Write(double aValue)
        {
            WriteSelected(aValue);

            return 0;
        }

    }
}
