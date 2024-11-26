
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/DigitalOutputModule.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/CLI/InstanceList.h>
#include <KMS/CLI/Module.h>
#include <KMS/DAQ/DigitalOutput.h>

namespace KMS
{
    namespace DAQ
    {

        class DigitalOutputModule : public CLI::Module
        {

        public:

            DigitalOutputModule();

            // Exception  RESULT_INVALID_STATE
            void DisplaySelected(std::ostream& aOut);

            void List(std::ostream& aOut) const;

            unsigned int List(std::ostream& aOut, const char* aRegEx) const;

            // Exception  RESULT_INVALID_STATE
            void SetSelected(bool aValue);

            // ===== ICommandParser =========================================
            virtual int ExecuteCommand(CLI::CommandLine* aCmd);

            // ===== Module =================================================
            virtual void DisplayHelp(FILE* aOut) const;

            CLI::InstanceList<DigitalOutput> mDigitalOutputs;

        private:

            int Cmd        (CLI::CommandLine* aCmd);
            int Cmd_Display(CLI::CommandLine* aCmd);
            int Cmd_List   (CLI::CommandLine* aCmd);
            int Cmd_Set    (CLI::CommandLine* aCmd);

        };

    }
}

