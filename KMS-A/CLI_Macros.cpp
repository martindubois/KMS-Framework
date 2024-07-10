
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_Macros.cpp

// TEST COVERAGE 2024-07-10 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
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

        int Macros::ExecuteCommand(const char* aCmd)
        {
            int lResult = -1;

            char lCmd [LINE_LENGTH];
            char lName[NAME_LENGTH];

            if      (0 == strcmp("Macro Display", aCmd)) { lResult = Cmd_Display(); }
            else if (1 == sscanf_s(aCmd, "Macro Add %s"  , lName SizeInfo(lName))) { lResult = Cmd_Add (lName); }
            else if (1 == sscanf_s(aCmd, "Macro + %s"    , lCmd  SizeInfo(lCmd ))) { lResult = Cmd_Plus(lCmd ); }
            else if (1 == sscanf_s(aCmd, "Macro %[^\r\n]", lCmd  SizeInfo(lCmd ))) { lResult = mMacros.ExecuteCommand(lCmd); }
            else
            {
                auto lM = mMacros.GetByName_Try(aCmd);
                if (nullptr == lM)
                {
                    lResult = Module::UNKNOWN_COMMAND;
                }
                else
                {
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
                "Macro List\n"
                "Macro Select {Name}\n"
                "Macro + {Command}\n");
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int Macros::Cmd_Add(const char* aName)
        {
            auto lM = Add(aName);

            std::cout << aName << " addded" << std::endl;

            return 0;
        }

        int Macros::Cmd_Display()
        {
            auto lM = mMacros.GetSelected();
            assert(nullptr != lM);

            lM->Display(std::cout);

            return 0;
        }

        int Macros::Cmd_Plus(const char* aCmd)
        {
            auto lM = mMacros.GetSelected();
            assert(nullptr != lM);

            lM->AddCommand(aCmd);

            std::cout << "Added" << std::endl;

            return 0;
        }

    }
}
