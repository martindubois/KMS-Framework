
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_Tool.cpp

// TEST COVERAGE  2023-08-09  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== C ==================================================================
#include <signal.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Console/Color.h>
#include <KMS/DI/Functions.h>
#include <KMS/DI/String_Expand.h>
#include <KMS/Text/File_ASCII.h>

#include <KMS/CLI/Tool.h>

KMS_RESULT_STATIC(RESULT_INVALID_COMMAND);

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

        void Tool::AddCommand(const char* aC)
        {
            mCommands.AddEntry(new DI::String(aC), true);
        }

        void Tool::ExecuteCommands(FILE* aFile)
        {
            assert(nullptr != aFile);

            char lLine[LINE_LENGTH];

            if (stdin == aFile)
            {
                // NOT TESTED
                mConsole.OutputStream() << "\n> " << std::flush;
            }

            while (nullptr != fgets(lLine, sizeof(lLine), aFile))
            {
                // NOT TESTED
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
                    mConsole.OutputStream() << "\n> " << std::flush;
                }
            }
        }

        void Tool::ExecuteScript(const char* aName)
        {
            Text::File_ASCII lScript;

            lScript.Read(File::Folder::CURRENT, aName);

            lScript.RemoveEmptyLines();
            lScript.RemoveComments_Script();

            for (const auto& lLine : lScript.mLines)
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
            assert(nullptr != aOut);

            fprintf(aOut,
                "Config {Name} [Op] [Value]\n"
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
            char lValue[LINE_LENGTH];

            if      (0 == strcmp(aC, "Exit")) { mExit++; }
            else if (0 == strcmp(aC, "Help")) { DisplayHelp(mConsole.OutputFile()); }
            else if (0 == strcmp(aC, "Shell")) { ExecuteCommands(stdin); }
            else if (1 == sscanf_s(aC, "Config %[^\n\r\t]", lValue SizeInfo(lValue)))
            {
                Config(lValue);
            }
            else if (1 == sscanf_s(aC, "Delay %u", &lDelay_ms))
            {
                #ifdef _KMS_LINUX_
                    sleep(1 + lDelay_ms / 1000);
                #endif

                #ifdef _KMS_WINDOWS_
                    Sleep(lDelay_ms);
                #endif
            }
            else if (1 == sscanf_s(aC, "Echo %[^\n\r\t]", lValue SizeInfo(lValue))) { mConsole.OutputStream() << lValue << std::endl; }
            else if (1 == sscanf_s(aC, "ExecuteScript %[^\n\r\t]", lValue SizeInfo(lValue)))
            {
                ExecuteScript(lValue);
            }
            else if (2 == sscanf_s(aC, "Repeat %u %[^\n\r\t]", &lCount, lValue SizeInfo(lValue)))
            {
                Repeat(lCount, lValue);
            }
            else if (1 == sscanf_s(aC, "UntilCtrlC %[^\n\r\t]", lValue SizeInfo(lValue))) { UntilCtrlC(lValue); }
            else
            {
                KMS_EXCEPTION(RESULT_INVALID_COMMAND, "Invalid command", aC);
            }
        }

        int Tool::Run()
        {
            for (const auto& lEntry : mCommands.mInternal)
            {
                auto lCommand = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lCommand);

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

        // ===== DI::Container ==============================================

        void Tool::Validate() const
        {
            DI::Dictionary::Validate();

            for (auto& lEntry : mCommands.mInternal)
            {
                auto lCommand = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lCommand);

                KMS_EXCEPTION_ASSERT(!lCommand->GetString().empty(), RESULT_INVALID_CONFIG, "Empty command", lCommand->Get());
            }
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Tool::Tool() : mExit(0)
        {
            mCommands.SetCreator(DI::String_Expand::Create);

            AddEntry("Commands", &mCommands, false, &MD_COMMANDS);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Tool::Config(const char* aOperation)
        {
            if (!DI::Execute_Operation(this, aOperation))
            {
                mConsole.OutputStream() << Console::Color::YELLOW;
                mConsole.OutputStream() << "WARNING  Ignored operation" << Console::Color::WHITE << std::endl;
            }
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
            auto lHandler = signal(SIGINT, OnCtrlC);

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

// NOT TESTED
void OnCtrlC(int aSignal)
{
    assert(SIGINT == aSignal);

    std::cout << "Ctrl-C" << std::endl;

    sContinue = false;
}
