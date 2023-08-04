
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

        public:

            void AddCommand(const char* aC);

            void ExecuteCommands(FILE* aF);

            void ExecuteScript(const char* aFileName);

            virtual void DisplayHelp(FILE* aOut) const;

            // Exception  CLI_COMMAND_INVALID
            virtual void ExecuteCommand(const char* aC);

            virtual int Run();

        // Internal

            Console::Console mConsole;

        protected:

            Tool();

        private:

            NO_COPY(Tool);

            void Config(const char* aOperation);

            void Repeat(unsigned int aCount, const char* aC);

            void UntilCtrlC(const char* aC);

            unsigned int mExit;

            // ===== Configurable attributes ================================
            DI::Array mCommands;

        };

    }
}