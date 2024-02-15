
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Module.h
// Library   KMS-A

#pragma once

namespace KMS
{
    namespace CLI
    {

        class Module
        {

        public:

            static const int UNKNOWN_COMMAND;

            virtual void DisplayHelp(FILE* aOut) const = 0;

            // Return  0                OK
            //         UNKNOWN_COMMAND  Continue parsing
            //         Other            Error
            //
            // Exception  ...
            virtual int ExecuteCommand(const char* aC) = 0;

        protected:

            Module();

        };

    }
}
