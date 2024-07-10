
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_Macro.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/Macros.h>

namespace KMS
{
    namespace CLI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Macro::AddCommand(const char* aCmd)
        {
            assert(nullptr != aCmd);

            mCommands.push_back(aCmd);
        }

        void Macro::Display(std::ostream& aOut)
        {
            for (auto lC : mCommands)
            {
                aOut << "Macro + " << lC << "\n";
            }
        }

        int Macro::Execute(ICommandParser* aParser)
        {
            int lResult = __LINE__;

            for (auto lC : mCommands)
            {
                lResult = aParser->ExecuteCommand(lC.c_str());
                if (0 != lResult)
                {
                    break;
                }
            }

            return lResult;
        }

    }
}
