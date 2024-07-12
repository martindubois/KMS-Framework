
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

            // Exception  RESULT_INVALID_STATE
            void SetSelected(bool aValue);

            // ===== ICommandParser =========================================
            virtual int ExecuteCommand(const char* aC);

            // ===== Module =================================================
            virtual void DisplayHelp(FILE* aOut) const;

            CLI::InstanceList<DigitalOutput> mDigitalOutputs;

        private:

            int Cmd_Display();
            int Cmd_List();
            int Cmd_Set(const char* aValue);

        };

    }
}

