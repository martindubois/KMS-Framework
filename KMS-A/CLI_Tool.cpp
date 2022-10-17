
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_Tool.cpp

#include "Component.h"

// ===== C ==================================================================
#include <signal.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Text/File_ASCII.h>

#include <KMS/CLI/Tool.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_COMMANDS("Commands += {Command}");

// Variables
// //////////////////////////////////////////////////////////////////////////

static bool sContinue;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

extern "C"
{
    static void OnCtrlC(int aSignal);
}

namespace KMS
{
    namespace CLI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Tool::ExecuteCommands(FILE* aFile)
        {
            assert(NULL != aFile);

            char lLine[LINE_LENGTH];

            if (stdin == aFile)
            {
                std::cout << "\n> " << std::flush;
            }

            while (NULL != fgets(lLine, sizeof(lLine), aFile))
            {
                char lCmd[LINE_LENGTH];

                if (1 == sscanf_s(lLine, " %[^\n\r\t]", lCmd SizeInfo(lCmd)))
                {
                    try
                    {
                        ExecuteCommand(lCmd);
                    }
                    KMS_CATCH;

                    if (0 < mExit)
                    {
                        mExit--;
                        break;
                    }
                }

                if (stdin == aFile)
                {
                    std::cout << "\n> " << std::flush;
                }
            }
        }

        void Tool::ExecuteScript(const char* aName)
        {
            Text::File_ASCII lScript;

            lScript.Read(File::Folder::CURRENT, aName);

            lScript.RemoveEmptyLines();
            lScript.RemoveComments_Script();

            for (const std::string& lLine : lScript.mLines)
            {
                try
                {
                    ExecuteCommand(lLine.c_str());
                }
                KMS_CATCH;

                if (0 < mExit)
                {
                    mExit--;
                    break;
                }
            }
        }

        void Tool::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "Config {Name} {Value}\n"
                "Delay {Delay_ms}\n"
                "Echo {Message}\n"
                "Exit\n"
                "ExecuteScript {FileName}\n"
                "Help\n"
                "Repeat {Count} {Command}\n"
                "Shell\n"
                "UntilCtrlC {Command}\n");
        }

        void Tool::ExecuteCommand(const char* aC)
        {
            unsigned int lCount;
            unsigned int lDelay_ms;
            char lName[PATH_LENGTH];
            char lValue[LINE_LENGTH];

            if      (0 == strcmp(aC, "Exit")) { mExit++; }
            else if (0 == strcmp(aC, "Help")) { DisplayHelp(stdout); }
            else if (0 == strcmp(aC, "Shell")) { ExecuteCommands(stdin); }
            else if (2 == sscanf_s(aC, "Config %[^ \n\r\t] %[^\n\r\t]", lName SizeInfo(lName), lValue SizeInfo(lValue)))
            {
                Config(lName, lValue);
            }
            else if (1 == sscanf_s(aC, "Delay %u", &lDelay_ms)) { Sleep(lDelay_ms); }
            else if (1 == sscanf_s(aC, "Echo %[^\n\r\t]", lValue SizeInfo(lValue))) { std::cout << lValue << std::endl; }
            else if (1 == sscanf_s(aC, "ExecuteScript %[^\n\r\t]", lName SizeInfo(lName)))
            {
                ExecuteScript(lName);
            }
            else if (2 == sscanf_s(aC, "Repeat %u %[^\n\r\t]", &lCount, lValue SizeInfo(lValue)))
            {
                Repeat(lCount, lValue);
            }
            else if (1 == sscanf_s(aC, "UntilCtrlC %[^\n\r\t]", &lValue SizeInfo(lValue))) { UntilCtrlC(lValue); }
            else
            {
                KMS_EXCEPTION(CLI_COMMAND_INVALID, "Invalid command", aC);
            }
        }

        int Tool::Run()
        {
            for (const DI::Array::Entry& lEntry : mCommands.mInternal)
            {
                const DI::String* lCommand = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lCommand);

                try
                {
                    ExecuteCommand(lCommand->Get());
                }
                KMS_CATCH;

                if (0 < mExit)
                {
                    mExit--;
                    break;
                }
            }

            return 0;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Tool::Tool() : mExit(0)
        {
            mCommands.SetCreator(KMS::DI::String_Expand::Create);

            AddEntry("Commands", &mCommands, false, &MD_COMMANDS);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Tool::Config(const char* aName, const char* aValue)
        {
            DI::Object* lObject = GetEntry_RW(aName);
            KMS_EXCEPTION_ASSERT(NULL != lObject, CLI_NAME_INVALID, "Invalid attribute name", aName);

            DI::Value* lValue = dynamic_cast<DI::Value*>(lObject);
            KMS_EXCEPTION_ASSERT(NULL != lValue, CLI_NAME_INVALID, "The name is not associated to a value", aName);

            lValue->Set(aValue);
        }

        void Tool::Repeat(unsigned int aCount, const char* aC)
        {
            for (unsigned int i = 0; i < aCount; i++)
            {
                ExecuteCommand(aC);
            }
        }

        void Tool::UntilCtrlC(const char* aC)
        {
            _crt_signal_t lHandler = signal(SIGINT, OnCtrlC);

            sContinue = true;

            try
            {
                while (sContinue)
                {
                    ExecuteCommand(aC);
                }
            }
            KMS_CATCH;

            signal(SIGINT, lHandler);
        }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void OnCtrlC(int aSignal)
{
    assert(SIGINT == aSignal);

    std::cout << "Ctrl-C" << std::endl;

    sContinue = false;
}
