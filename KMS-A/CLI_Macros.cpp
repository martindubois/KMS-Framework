
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_Macros.cpp

// TEST COVERAGE 2024-07-10 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>

#include <KMS/CLI/Macros.h>

namespace KMS
{
    namespace CLI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Macros::Macros(ICommandParser* aParser) : mParser(aParser)
        {
            assert(nullptr != aParser);
        }

        Macro* Macros::Add(const char* aName)
        {
            auto lResult = new Macro;

            mMacros.Add(aName, lResult);

            return lResult;
        }

        // ===== ICommandParser =============================================

        int Macros::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            int lResult = -1;

            auto lCmd = aCmd->GetCurrent();

            if (0 == strcmp("Macro", lCmd)) { aCmd->Next(); lResult = Cmd(aCmd); }
            else
            {
                auto lM = mMacros.GetByName_Try(lCmd);
                if (nullptr == lM)
                {
                    lResult = Module::UNKNOWN_COMMAND;
                }
                else
                {
                    aCmd->Next();

                    KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

                    lResult = lM->Execute(mParser);
                }
            }

            return lResult;
        }

        // ===== Module =====================================================

        void Macros::DisplayHelp(FILE* aOut) const
        {
            fprintf(aOut,
                "Macro Add {Name}\n"
                "Macro Delete All\n"
                "Macro Delete\n"
                "Macro Display\n"
                "Macro List [RegEx]\n"
                "Macro Select {Name}\n"
                "Macro + {Command}\n");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int Macros::Cmd(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            int  lResult = __LINE__;

            if      (0 == _stricmp(lCmd, "Add"    )) { aCmd->Next(); lResult = Cmd_Add    (aCmd); }
            else if (0 == _stricmp(lCmd, "Display")) { aCmd->Next(); lResult = Cmd_Display(aCmd); }
            else if (0 == _stricmp(lCmd, "+"      )) { aCmd->Next(); lResult = Cmd_Plus   (aCmd); }
            else
            {
                lResult = mMacros.ExecuteCommand(aCmd);
            }

            return lResult;
        }

        int Macros::Cmd_Add(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lName = aCmd->GetCurrent();

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            auto lM = Add(lName);

            std::cout << lName << " addded" << std::endl;

            return 0;
        }

        int Macros::Cmd_Display(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            auto lM = mMacros.GetSelected();
            assert(nullptr != lM);

            lM->Display(std::cout);

            return 0;
        }

        int Macros::Cmd_Plus(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            char lCmd[LINE_LENGTH];

            aCmd->GetRemaining(lCmd, sizeof(lCmd));

            auto lM = mMacros.GetSelected();
            assert(nullptr != lM);

            lM->AddCommand(lCmd);

            std::cout << "Added" << std::endl;

            return 0;
        }

    }
}
