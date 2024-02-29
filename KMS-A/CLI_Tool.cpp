
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_Tool.cpp

// TEST COVERAGE 2023-10-02 Martin Dubois

#include "Component.h"

// ===== C ==================================================================
#include <signal.h>

// ===== C++ ================================================================
#include <thread>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Console/Color.h>
#include <KMS/Dbg/Log.h>
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

        void Tool::AddModule(Module* aModule)
        {
            assert(nullptr != aModule);

            mModules.push_back(aModule);
        }

        void Tool::ClearError() { mError_Code = 0; mError_Command = ""; }

        int Tool::GetExitCode()
        {
            if (0 == mExit_Code)
            {
                mExit_Code = mError_Code;

                mError_Code = 0;
            }

            return mExit_Code;
        }

        int Tool::ExecuteCommands(FILE* aFile)
        {
            assert(nullptr != aFile);

            char lLine[LINE_LENGTH];

            if (stdin == aFile)
            {
                // NOT TESTED
                std::cout << "\n> " << std::flush;
            }

            int lResult = 0;

            while (nullptr != fgets(lLine, sizeof(lLine), aFile))
            {
                // NOT TESTED
                char lCmd[LINE_LENGTH];

                if (1 == sscanf_s(lLine, " %[^\n\r\t]", lCmd SizeInfo(lCmd)))
                {
                    CallExecuteCommand(lCmd);

                    if (0 < mExit_Count)
                    {
                        ExitCodeToErrorCode();
                        break;
                    }
                }

                if (stdin == aFile)
                {
                    std::cout << "\n> " << std::flush;
                }
            }

            return GetExitCode();
        }

        int Tool::ExecuteScript(const char* aName)
        {
            Text::File_ASCII lScript;

            lScript.Read(File::Folder::CURRENT, aName);

            lScript.RemoveEmptyLines();
            lScript.RemoveComments_Script();

            for (const auto& lLine : lScript.mLines)
            {
                CallExecuteCommand(lLine.c_str());

                if (0 < mExit_Count)
                {
                    ExitCodeToErrorCode();
                    break;
                }
            }

            return GetExitCode();
        }

        void Tool::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            for (auto lModule : mModules)
            {
                assert(nullptr != lModule);

                lModule->DisplayHelp(aOut);
            }

            fprintf(aOut,
                "AbortIfError\n"
                "ChangeDir\n"
                "ClearError\n"
                "Config {Name} [Op] [Value]\n"
                "Delay {Delay_ms}\n"
                "Echo {Message}\n"
                "ExecuteScript {FileName}\n"
                "Exit [Code]\n"
                "ExitIfError\n"
                "Help\n"
                "Repeat {Count} {Command}\n"
                "Shell\n"
                "UntilCtrlC {Command}\n");
        }

        int Tool::ExecuteCommand(const char* aC)
        {
            for (auto lModule : mModules)
            {
                assert(nullptr != lModule);

                auto lRet = lModule->ExecuteCommand(aC);
                if (Module::UNKNOWN_COMMAND != lRet)
                {
                    return lRet;
                }
            }

            unsigned int lCount;
            unsigned int lDelay_ms;
            int          lResult = 0;
            char lValue[LINE_LENGTH];
            
            if      (0 == strcmp(aC, "AbortIfError")) { AbortIfError(); }
            else if (1 == sscanf_s(aC, "ChangeDir %[^\n\r\t]", lValue SizeInfo(lValue)))
            {
                ChangeDir(lValue);
            }
            else if (0 == strcmp(aC, "ClearError"  )) { ClearError  (); }
            else if (1 == sscanf_s(aC, "Config %[^\n\r\t]", lValue SizeInfo(lValue)))
            {
                lResult = Config(lValue);
            }
            else if (1 == sscanf_s(aC, "Delay %u", &lDelay_ms))
            {
                Delay(lDelay_ms);
            }
            else if (1 == sscanf_s(aC, "Echo %[^\n\r\t]", lValue SizeInfo(lValue))) { std::cout << lValue << std::endl; }
            else if (1 == sscanf_s(aC, "ExecuteScript %[^\n\r\t]", lValue SizeInfo(lValue)))
            {
                lResult = ExecuteScript(lValue);
            }
            else if (0 == strcmp(aC, "ExitIfError")) { ExitIfError(); }
            else if (1 == sscanf_s(aC, "Exit %d", &mExit_Code)) { mExit_Count++; }
            else if (0 == strcmp(aC, "Exit")) { mExit_Count++; }
            else if (0 == strcmp(aC, "Help")) { DisplayHelp(stdout); }
            else if (2 == sscanf_s(aC, "Repeat %u %[^\n\r\t]", &lCount, lValue SizeInfo(lValue)))
            {
                lResult = Repeat(lCount, lValue);
            }
            else if (0 == strcmp(aC, "Shell")) { lResult = ExecuteCommands(stdin); }
            else if (1 == sscanf_s(aC, "UntilCtrlC %[^\n\r\t]", lValue SizeInfo(lValue))) { lResult = UntilCtrlC(lValue); }
            else
            {
                KMS_EXCEPTION(RESULT_INVALID_COMMAND, "Invalid command", aC);
            }

            return lResult;
        }

        int Tool::Run()
        {
            for (const auto& lEntry : mCommands.mInternal)
            {
                auto lCommand = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lCommand);

                CallExecuteCommand(lCommand->Get());

                if (0 < mExit_Count)
                {
                    ExitCodeToErrorCode();
                    break;
                }
            }

            return GetExitCode();
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

        Tool::Tool() : mError_Code(0), mExit_Code(0), mExit_Count(0)
        {
            mCommands.SetCreator(DI::String_Expand::Create);

            AddEntry("Commands", &mCommands, false, &MD_COMMANDS);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Tool::AbortIfError()
        {
            if (0 != mError_Code)
            {
                // NOT TESTED
                KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
                char lMsg[64];
                sprintf_s(lMsg, "Abort on error (%d)", mError_Code);
                Dbg::gLog.WriteMessage(lMsg);

                exit(mError_Code);
            }
        }

        void Tool::ChangeDir(const char* aDir)
        {
            File::Folder::ChangeCurrentDirectory(File::Folder(File::Folder::NONE, aDir));
        }

        int Tool::Config(const char* aOperation)
        {
            int lResult = 0;

            if (!DI::Execute_Operation(this, aOperation))
            {
                lResult = __LINE__;
            }

            return lResult;
        }

        void Tool::Delay(unsigned int aDelay_ms)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(aDelay_ms));
        }

        void Tool::ExitIfError()
        {
            if (0 != mError_Code)
            {
                // NOT TESTED
                KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
                char lMsg[64];
                sprintf_s(lMsg, "Exit on error (%d)", mError_Code);
                Dbg::gLog.WriteMessage(lMsg);

                mExit_Code  = mError_Code;
                mError_Code = 0;

                mExit_Count++;
            }
        }

        int Tool::Repeat(unsigned int aCount, const char* aC)
        {
            int lResult = 0;

            for (unsigned int i = 0; i < aCount; i++)
            {
                if (!CallExecuteCommand(aC))
                {
                    lResult = mError_Code;
                    break;
                }
            }

            return lResult;
        }

        int Tool::UntilCtrlC(const char* aC)
        {
            sContinue = true;

            auto lHandler = signal(SIGINT, OnCtrlC);
            int  lResult  = 0;

            while (sContinue)
            {
                if (!CallExecuteCommand(aC))
                {
                    lResult = mError_Code;
                    break;
                }
            }

            signal(SIGINT, lHandler);

            return lResult;
        }

        bool Tool::CallExecuteCommand(const char* aC)
        {
            assert(nullptr != aC);

            char lMsg[64 + LINE_LENGTH];
            auto lResult = false;

            try
            {
                auto lRet = ExecuteCommand(aC);
                if (0 == lRet)
                {
                    lResult = true;
                }
                else
                {
                    KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);
                    sprintf_s(lMsg, "The command \"%s\" returned %d", aC, lRet);
                    KMS::Dbg::gLog.WriteMessage(lMsg);
                    mError_Code = lRet;
                }
            }
            catch (Exception eE)
            {
                KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);
                sprintf_s(lMsg, "The command \"%s\" raised an Exception \"%s\"", aC, eE.what());
                KMS::Dbg::gLog.WriteMessage(lMsg);
                mError_Code = eE.GetCode();
            }
            catch (std::exception eE)
            {
                KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);
                sprintf_s(lMsg, "The command \"%s\" raised a std::exception \"%s\"", aC, eE.what());
                KMS::Dbg::gLog.WriteMessage(lMsg);
                mError_Code = __LINE__;
            }
            catch (...)
            {
                KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);
                sprintf_s(lMsg, "The command \"%s\" raised an unknown exception", aC);
                KMS::Dbg::gLog.WriteMessage(lMsg);
                mError_Code = __LINE__;
            }

            if (!lResult)
            {
                mError_Command = aC;
            }

            return lResult;
        }

        void Tool::ExitCodeToErrorCode()
        {
            assert(0 < mExit_Count);

            mError_Code    = mExit_Code;
            mError_Command = "";

            mExit_Code  = 0;

            mExit_Count--;
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
