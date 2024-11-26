
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS-A/DI_ValueModule.h

#include "Component.h"

// ===== C++ ================================================================
#include <regex>

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>

#include <KMS/DI/ValueModule.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        ValueModule::ValueModule(const char* aName, bool aReadWrite) : mName(aName), mReadWrite(aReadWrite)
        {
            assert(nullptr != aName);

            mValues.SetAllowedCmds(CLI::InstanceList_Base::CMD_SELECT);
        }

        void ValueModule::DisplaySelected(std::ostream& aOut)
        {
            auto lV = mValues.GetSelected();
            assert(nullptr != lV);

            char lText[LINE_LENGTH];

            lV->Get(lText, sizeof(lText));

            aOut << lText << std::endl;
        }

        void ValueModule::List(std::ostream& aOut) const
        {
            for (auto lPair : mValues.mInstances)
            {
                assert(nullptr != lPair.second);

                char lText[LINE_LENGTH];

                reinterpret_cast<Value*>(lPair.second)->Get(lText, sizeof(lText));

                aOut << lPair.first << "\t" << lText << "\n";
            }

            aOut << std::endl;
        }

        unsigned int ValueModule::List(std::ostream& aOut, const char* aRegEx) const
        {
            assert(nullptr != aRegEx);

            unsigned int lResult = 0;

            std::regex lRegEx(aRegEx);

            for (auto lPair : mValues.mInstances)
            {
                assert(nullptr != lPair.second);

                if (std::regex_match(lPair.first, lRegEx))
                {
                    char lText[LINE_LENGTH];

                    reinterpret_cast<Value*>(lPair.second)->Get(lText, sizeof(lText));

                    aOut << lPair.first << "\t" << lText << "\n";

                    lResult++;
                }
            }

            aOut << std::endl;

            return lResult;
        }

        void ValueModule::SetSelected(const char* aValue)
        {
            auto lV = mValues.GetSelected();
            assert(nullptr != lV);

            lV->Set(aValue);
        }

        // ===== CLI::ICommandParser ========================================

        int ValueModule::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            assert(nullptr != mName);

            auto lCmd    = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            assert(nullptr != lCmd);

            if (0 == _stricmp(lCmd, mName)) { aCmd->Next(); lResult = Cmd(aCmd); }

            return lResult;
        }

        // ===== CLI::Module ================================================
        void ValueModule::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            assert(nullptr != mName);

            fprintf(aOut,
                "%s Display\n"
                "%s List [RegEx]\n"
                "%s Select {Name}\n",
                mName,
                mName,
                mName);

            if (mReadWrite)
            {
                fprintf(aOut,
                    "%s Set {Value}\n",
                    mName);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int ValueModule::Cmd(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd    = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if      (0 == _stricmp(lCmd, "Display")) { aCmd->Next(); lResult = Cmd_Display(aCmd); }
            else if (0 == _stricmp(lCmd, "List"   )) { aCmd->Next(); lResult = Cmd_List   (aCmd); }
            else if (mReadWrite && (0 == _stricmp(lCmd, "Set"))) { aCmd->Next(); lResult = Cmd_Set(aCmd); }
            else
            {
                lResult = mValues.ExecuteCommand(aCmd);
            }

            return lResult;
        }

        int ValueModule::Cmd_Display(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            DisplaySelected(std::cout);

            return 0;
        }

        int ValueModule::Cmd_List(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            if (aCmd->IsAtEnd())
            {
                List(std::cout);
            }
            else
            {
                char lText[LINE_LENGTH];

                aCmd->GetRemaining(lText, sizeof(lText));

                auto lCount = List(std::cout, lText);

                std::cout << lCount << " of ";
            }

            std::cout << mValues.mInstances.size() << " values" << std::endl;

            return 0;
        }

        int ValueModule::Cmd_Set(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            char lText[LINE_LENGTH];

            aCmd->GetRemaining(lText, sizeof(lText));

            SetSelected(lText);

            return 0;
        }

    }
}
