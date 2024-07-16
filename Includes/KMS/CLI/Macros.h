
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Macros.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <list>
#include <string>

// ===== Includes ===========================================================
#include <KMS/CLI/ICommandParser.h>
#include <KMS/CLI/InstanceList.h>
#include <KMS/CLI/Module.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace CLI
    {

        class Macro
        {

        public:

            void AddCommand(const char* aCmd);

            void Display(std::ostream& aOut);

            int Execute(ICommandParser* aParser);

        private:

            StringList_ASCII mCommands;

        };

        class Macros : public Module
        {

        public:

            Macros(ICommandParser* aParser);

            Macro* Add(const char* aName);

            // ===== ICommandParser =========================================
            virtual int ExecuteCommand(CLI::CommandLine* aCmd);

            // ===== Module =================================================
            virtual void DisplayHelp(FILE* aOut) const;

            InstanceList<Macro> mMacros;

        private:

            int Cmd        (CLI::CommandLine* aCmd);
            int Cmd_Add    (CLI::CommandLine* aCmd);
            int Cmd_Display(CLI::CommandLine* aCmd);
            int Cmd_Plus   (CLI::CommandLine* aCmd);

            ICommandParser* mParser;

        };

    }
}
