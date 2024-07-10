
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Module.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/CLI/ICommandParser.h>

namespace KMS
{
    namespace CLI
    {

        class Module : public ICommandParser
        {

        public:

            static const int UNKNOWN_COMMAND;

            virtual void DisplayHelp(FILE* aOut) const = 0;

        protected:

            Module();

        };

    }
}
