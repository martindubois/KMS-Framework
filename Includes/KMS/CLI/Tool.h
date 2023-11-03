
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Banner.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Console/Console.h>
#include <KMS/DI/Array.h>
#include <KMS/DI/Dictionary.h>

namespace KMS
{
    namespace CLI
    {

        class Tool : public DI::Dictionary
        {

        private:

            DI::Array mCommands;

        public:

            void AddCommand(const char* aC);

            void ClearError();

            int GetExitCode();

            int ExecuteCommands(FILE* aF);

            int ExecuteScript(const char* aFileName);

            virtual void DisplayHelp(FILE* aOut) const;

            // Exception  CLI_COMMAND_INVALID
            virtual int ExecuteCommand(const char* aC);

            virtual int Run();

            // ===== DI::Container ==========================================
            virtual void Validate() const;

        // Internal

            Console::Console mConsole;

        protected:

            Tool();

        private:

            NO_COPY(Tool);

            void AbortIfError();

            int Config(const char* aOperation);

            void Delay(unsigned int aDelay_ms);

            void ExitIfError();

            int Repeat(unsigned int aCount, const char* aC);

            int UntilCtrlC(const char* aC);

            bool CallExecuteCommand(const char* aC);

            void ExitCodeToErrorCode();

            int         mError_Code;
            std::string mError_Command;

            int          mExit_Code;
            unsigned int mExit_Count;

        };

    }
}
