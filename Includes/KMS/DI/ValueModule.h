
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/ValueModule.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/CLI/InstanceList.h>
#include <KMS/CLI/Module.h>
#include <KMS/DI/Value.h>

namespace KMS
{
    namespace DI
    {

        class ValueModule : public CLI::Module
        {

        public:

            ValueModule(const char* aName = "Value", bool aReadWrite = false);

            // Exception  RESULT_INVALID_STATE
            void DisplaySelected(std::ostream& aOut);

            void List(std::ostream& aOut) const;

            // ===== ICommandParser =========================================
            virtual int ExecuteCommand(CLI::CommandLine* aCmd);

            // ===== Module =================================================
            virtual void DisplayHelp(FILE* aOut) const;

            CLI::InstanceList<Value> mValues;

        private:

            int Cmd        (CLI::CommandLine* aCmd);
            int Cmd_Display(CLI::CommandLine* aCmd);
            int Cmd_List   (CLI::CommandLine* aCmd);
            int Cmd_Set    (CLI::CommandLine* aCmd);

            const char* mName;
            bool        mReadWrite;

        };

    }
}
