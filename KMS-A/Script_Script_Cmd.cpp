
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Script_Script_Cmd.cpp

// CODE REVIEW 2026-01-20 Martin Dubois
// TEST COVERAGE 2026-01-20 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Script/Script_Cmd.h>

namespace KMS
{
    namespace Script
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Script_Cmd::Script_Cmd(const char* aFileName) : Script(aFileName, ".cmd")
        {
            Write_Header();
        }

        // ===== Script =====================================================

        Script_Cmd::~Script_Cmd() { Write_Footer(); }

        void Script_Cmd::Write_Comment(const char* aComment)
        {
            assert(nullptr != aComment);

            std::string lLine = "rem ";

            lLine += aComment;

            Write_Line(lLine);
        }

        void Script_Cmd::Write_Echo(const std::string& aMessage, unsigned int aFlags)
        {
            char lMessage[LINE_LENGTH];

            Process_String(aMessage.c_str(), lMessage, sizeof(lMessage), aFlags | FLAG_MESSAGE);

            std::string lCmd = "echo ";

            lCmd += lMessage;

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        void Script_Cmd::Write_Exit(int aCode)
        {
            std::string lCmd = "exit /B ";

            lCmd += std::to_string(aCode);

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        void Script_Cmd::Write_If(const std::string& aCondition, unsigned int aFlags)
        {
            char lCondition[LINE_LENGTH];

            Process_String(aCondition.c_str(), lCondition, sizeof(lCondition), aFlags |= FLAG_COMMAND);

            std::string lCmd = "if ";

            lCmd += aCondition;
            lCmd += " (";

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);

            Indent_Inc();
        }

        void Script_Cmd::Write_If_Error() { Write_If("ERRORLEVEL 1", FLAG_DO_NOT_PROCESS); }

        void Script_Cmd::Write_If_NotExist(const char* aFileName, unsigned int aFlags)
        {
            assert(nullptr != aFileName);

            char lFileName[PATH_LENGTH];

            Process_String(aFileName, lFileName, sizeof(lFileName), aFlags | FLAG_FILE_NAME);

            std::string lCond = "not exist ";

            lCond += lFileName;

            Write_If(lCond, FLAG_DO_NOT_PROCESS);
        }

        void Script_Cmd::Write_Pause() { Write_Command("pause", FLAG_DO_NOT_PROCESS); }

        void Script_Cmd::Write_Variable_Set(const char* aVariable, const char* aValue, unsigned int aFlags)
        {
            assert(nullptr != aVariable);
            assert(nullptr != aValue);

            char lValue[LINE_LENGTH];

            Process_String(aValue, lValue, sizeof(lValue), aFlags);

            std::string lCmd = "set ";

            lCmd += aVariable;
            lCmd += "=";
            lCmd += lValue;

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // ===== Script =====================================================

        void Script_Cmd::Indent_Dec()
        {
            Script::Indent_Dec();

            Write_Line(")");
            Write_EmptyLine();
        }

        void Script_Cmd::Process_String(const char* aIn, char* aOut, unsigned int aOutSize_byte, unsigned int aFlags)
        {
            assert(nullptr != aIn);
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);

            if (0 != (aFlags & FLAG_DO_NOT_PROCESS))
            {
                strcpy_s(aOut SizeInfoV(aOutSize_byte), aIn);
            }
            else
            {
                auto lDoNotQuote = 0 != (aFlags & FLAG_DO_NOT_QUOTE);
                auto lFileName   = 0 != (aFlags & FLAG_FILE_NAME);
                auto lMessage    = 0 != (aFlags & FLAG_MESSAGE);
                auto lSpace = nullptr != strchr(aIn, ' ');
                auto lQuote = ((lFileName && lSpace) || lMessage) && (!lDoNotQuote);

                unsigned int lIn  = 0;
                unsigned int lOut = 0;

                if (lQuote)
                {
                    aOut[lOut] = '"'; lOut++;
                }

                while (aOutSize_byte > lOut)
                {
                    switch (aIn[lIn])
                    {
                    case '\0':
                        if (lQuote)
                        {
                            aOut[lOut] = '"'; lOut++;
                        }
                        aOut[lOut] = '\0'; lOut++;
                        return;

                    case '/':
                        aOut[lOut] = lFileName ? '\\' : aIn[lIn]; lOut++;
                        break;

                    default: aOut[lOut] = aIn[lIn]; lOut++;
                    }

                    lIn++;
                }

                KMS_EXCEPTION(RESULT_OUTPUT_TOO_SHORT, "String too long (NOT TESTED)", aIn);
            }
        }

        void Script_Cmd::Write_Footer()
        {
            Script::Write_Footer();

            Write_Exit(0);
        }

        void Script_Cmd::Write_Header()
        {
            Write_Command("@echo off", FLAG_DO_NOT_PROCESS);

            Script::Write_Header();
        }

    }
}
