
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/DigitalInputModule.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/CLI/InstanceList.h>
#include <KMS/CLI/Module.h>
#include <KMS/DAQ/DigitalInput.h>

namespace KMS
{
    namespace DAQ
    {

        class DigitalInputModule : public CLI::Module
        {

        public:

            DigitalInputModule();

            // Exception  RESULT_INVALID_STATE
            void DisplaySelected(std::ostream& aOut);

            void List(std::ostream& aOut) const;

            // ===== ICommandParser =========================================
            virtual int ExecuteCommand(const char* aC);

            // ===== Module =================================================
            virtual void DisplayHelp(FILE* aOut) const;

            CLI::InstanceList<DigitalInput> mDigitalInputs;

        private:

            int Cmd_Display();
            int Cmd_List();

        };

    }
}

