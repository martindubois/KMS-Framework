
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
#include <KMS/CLI/CommandLine.h>
#include <KMS/Console/Color.h>
#include <KMS/Convert.h>
#include <KMS/Dbg/Log.h>
#include <KMS/DI/Functions.h>
#include <KMS/DI/String_Expand.h>
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

        void Tool::AddCommand(const char* aC)
        {
            Ptr_OF<DI::Object> lEntry(new DI::String(aC), true);

            mCommands.AddEntry(lEntry);
        }

        void Tool::AddModule(Module* aModule)
        {
            assert(nullptr != aModule);

            mModules.push_back(aModule);
        }

        void Tool::ClearError() { mError_Code = 0; }

        int Tool::GetExitCode()
        {
            if (0 == mExit_Code)
            {
                mExit_Code = mError_Code;

                ClearError();
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

        int Tool::ToggleError()
        {
            if (0 == mError_Code)
            {
                return __LINE__;
            }

            ClearError();

            return 0;
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
                "AbortIfNoError\n"
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
                "ToggleError\n"
                "UntilCtrlC {Command}\n");
        }

        int Tool::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            for (auto lModule : mModules)
            {
                assert(nullptr != lModule);

                auto lRet = lModule->ExecuteCommand(aCmd);
                if (Module::UNKNOWN_COMMAND != lRet)
                {
                    return lRet;
                }
            }

            int  lResult = 0;

            auto lCmd = aCmd->GetCurrent();
            
            if      (0 == _stricmp(lCmd, "AbortIfError" )) { aCmd->Next(); lResult = Cmd_AbortIfError (aCmd); }
            else if (0 == _stricmp(lCmd, "AbortIfNoError")) { aCmd->Next(); lResult = Cmd_AbortIfNoError(aCmd); }
            else if (0 == _stricmp(lCmd, "ChangeDir"    )) { aCmd->Next(); lResult = Cmd_ChangeDir    (aCmd); }
            else if (0 == _stricmp(lCmd, "ClearError"   )) { aCmd->Next(); lResult = Cmd_ClearError   (aCmd); }
            else if (0 == _stricmp(lCmd, "Config"       )) { aCmd->Next(); lResult = Cmd_Config       (aCmd); }
            else if (0 == _stricmp(lCmd, "Delay"        )) { aCmd->Next(); lResult = Cmd_Delay        (aCmd); }
            else if (0 == _stricmp(lCmd, "Echo"         )) { aCmd->Next(); lResult = Cmd_Echo         (aCmd); }
            else if (0 == _stricmp(lCmd, "ExecuteScript")) { aCmd->Next(); lResult = Cmd_ExecuteScript(aCmd); }
            else if (0 == _stricmp(lCmd, "ExitIfError"  )) { aCmd->Next(); lResult = Cmd_ExitIfError  (aCmd); }
            else if (0 == _stricmp(lCmd, "Exit"         )) { aCmd->Next(); lResult = Cmd_Exit         (aCmd); }
            else if (0 == _stricmp(lCmd, "Help"         )) { aCmd->Next(); lResult = Cmd_Help         (aCmd); }
            else if (0 == _stricmp(lCmd, "Repeat"       )) { aCmd->Next(); lResult = Cmd_Repeat       (aCmd); }
            else if (0 == _stricmp(lCmd, "Shell"        )) { aCmd->Next(); lResult = Cmd_Shell        (aCmd); }
            else if (0 == _stricmp(lCmd, "ToggleError"  )) { aCmd->Next(); lResult = Cmd_ToggleError  (aCmd); }
            else if (0 == _stricmp(lCmd, "UntilCtrlC"   )) { aCmd->Next(); lResult = Cmd_UntilCtrlC   (aCmd); }
            else
            {
                KMS_EXCEPTION(RESULT_INVALID_COMMAND, "Invalid command", lCmd);
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

            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mCommands, false); AddEntry("Commands", lEntry, &MD_COMMANDS);
        }

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

        // Private
        // //////////////////////////////////////////////////////////////////

        int Tool::Cmd_AbortIfError(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            AbortIfError();

            return 0;
        }

        int Tool::Cmd_AbortIfNoError(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            auto lResult = ToggleError();
            if (0 != lResult)
            {
                KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);
                KMS::Dbg::gLog.WriteMessage("An error was expected");
                mError_Code = lResult;
            }

            AbortIfError();

            return lResult;
        }

        int Tool::Cmd_ChangeDir(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lDir = aCmd->GetCurrent();

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            File::Folder::ChangeCurrentDirectory(File::Folder(File::Folder::NONE, lDir));

            return 0;
        }

        int Tool::Cmd_ClearError(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            ClearError();

            return 0;
        }

        int Tool::Cmd_Config(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            int lResult = 0;

            char lOperation[LINE_LENGTH];

            aCmd->GetRemaining(lOperation, sizeof(lOperation));

            if (!DI::Execute_Operation(this, lOperation))
            {
                lResult = __LINE__;
            }

            return lResult;
        }

        int Tool::Cmd_Delay(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lDelay_ms = Convert::ToUInt32(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            std::this_thread::sleep_for(std::chrono::milliseconds(lDelay_ms));

            return 0;
        }

        int Tool::Cmd_Echo(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            while (!aCmd->IsAtEnd())
            {
                std::cout << aCmd->GetCurrent() << " ";

                aCmd->Next();
            }

            std::cout << std::endl;

            return 0;
        }

        int Tool::Cmd_ExecuteScript(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lScript = aCmd->GetCurrent();

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            return ExecuteScript(lScript);
        }

        int Tool::Cmd_Exit(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            if (aCmd->IsAtEnd())
            {
                exit(0);
            }
            else
            {
                auto lCode = Convert::ToInt32(aCmd->GetCurrent());

                aCmd->Next();

                KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

                exit(lCode);
            }

            return 0;
        }

        int Tool::Cmd_ExitIfError(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            if (0 != mError_Code)
            {
                // NOT TESTED
                KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
                char lMsg[64];
                sprintf_s(lMsg, "Exit on error (%d)", mError_Code);
                Dbg::gLog.WriteMessage(lMsg);

                mExit_Code  = mError_Code;

                ClearError();

                mExit_Count++;
            }

            return 0;
        }

        int Tool::Cmd_Help(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            DisplayHelp(stdout);

            return 0;
        }

        int Tool::Cmd_Repeat(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            int lResult = 0;

            auto lCount = Convert::ToUInt32(aCmd->GetCurrent());

            aCmd->Next();

            char lCmd[LINE_LENGTH];

            aCmd->GetRemaining(lCmd, sizeof(lCmd));

            for (unsigned int i = 0; i < lCount; i++)
            {
                if (!CallExecuteCommand(lCmd))
                {
                    lResult = mError_Code;
                    break;
                }
            }

            return lResult;
        }

        int Tool::Cmd_Shell(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            return ExecuteCommands(stdin);
        }

        int Tool::Cmd_ToggleError(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            return ToggleError();
        }

        int Tool::Cmd_UntilCtrlC(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            sContinue = true;

            auto lHandler = signal(SIGINT, OnCtrlC);
            int  lResult  = 0;

            char lCmd[LINE_LENGTH];
            
            aCmd->GetRemaining(lCmd, sizeof(lCmd));

            while (sContinue)
            {
                if (!CallExecuteCommand(lCmd))
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
                CLI::CommandLine lCmd(aC);

                auto lRet = ExecuteCommand(&lCmd);
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

            return lResult;
        }

        void Tool::ExitCodeToErrorCode()
        {
            assert(0 < mExit_Count);

            mError_Code    = mExit_Code;

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
