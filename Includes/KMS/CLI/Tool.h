
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Tool.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/CLI/ICommandParser.h>
#include <KMS/CLI/Module.h>
#include <KMS/DI/Array.h>
#include <KMS/DI/Dictionary.h>

namespace KMS
{
    namespace CLI
    {

        class Tool : public DI::Dictionary, public ICommandParser
        {

        private:

            DI::Array mCommands;

        public:

            void AddCommand(const char* aC);

            // The modules parse the command before the the base class parse
            // the default command. This way, a module could replace a default
            // command.
            void AddModule(Module* aModule);

            void ClearError();

            int GetExitCode();

            int ExecuteCommands(FILE* aF);

            int ExecuteScript(const char* aFileName);

            int ToggleError();

            virtual void DisplayHelp(FILE* aOut) const;

            virtual int Run();

            // ===== ICommandParser =========================================
            virtual int ExecuteCommand(CLI::CommandLine* aCmdLine);

            // ===== DI::Container ==========================================
            virtual void Validate() const;

        protected:

            Tool();

            void AbortIfError();

        private:

            typedef std::list<Module*> ModuleList;

            NO_COPY(Tool);

            int Cmd_AbortIfError (CLI::CommandLine* aCmd);
            int Cmd_AbortIfNoError(CLI::CommandLine* aCmd);
            int Cmd_ChangeDir    (CLI::CommandLine* aCmd);
            int Cmd_ClearError   (CLI::CommandLine* aCmd);
            int Cmd_Config       (CLI::CommandLine* aCmd);
            int Cmd_Delay        (CLI::CommandLine* aCmd);
            int Cmd_Echo         (CLI::CommandLine* aCmd);
            int Cmd_ExecuteScript(CLI::CommandLine* aCmd);
            int Cmd_Exit         (CLI::CommandLine* aCmd);
            int Cmd_ExitIfError  (CLI::CommandLine* aCmd);
            int Cmd_Help         (CLI::CommandLine* aCmd);
            int Cmd_Repeat       (CLI::CommandLine* aCmd);
            int Cmd_Shell        (CLI::CommandLine* aCmd);
            int Cmd_ToggleError  (CLI::CommandLine* aCmd);
            int Cmd_UntilCtrlC   (CLI::CommandLine* aCmd);

            bool CallExecuteCommand(const char* aC);

            void ExitCodeToErrorCode();

            int         mError_Code;

            int          mExit_Code;
            unsigned int mExit_Count;

            ModuleList mModules;

        };

    }
}
