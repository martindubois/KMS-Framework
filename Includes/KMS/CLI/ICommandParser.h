
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/ICommandParser.h

#pragma once

namespace KMS
{
    namespace CLI
    {

        class CommandLine;

        class ICommandParser
        {

        public:

            // Exception  CLI_COMMAND_INVALID
            virtual int ExecuteCommand(CLI::CommandLine* aCmd) = 0;

        };

    }
}
