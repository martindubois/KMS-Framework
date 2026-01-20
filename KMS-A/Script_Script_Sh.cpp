
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Script_Script_Sh.cpp

// CODE REVIEW 2026-01-20 Martin Dubois
// TEST COVERAGE 2026-01-20 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Script/Script_Sh.h>

namespace KMS
{
    namespace Script
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Script_Sh::Script_Sh(const char* aFileName) : Script(aFileName, ".sh")
        {
            Write_Header();
        }

        // ===== Script =====================================================

        Script_Sh::~Script_Sh() { Write_Footer(); }

        void Script_Sh::Write_Comment(const char* aComment)
        {
            assert(nullptr != aComment);

            std::string lComment = "# ";

            lComment += aComment;

            Write_Line(lComment);
        }

        void Script_Sh::Write_Echo(const std::string& aMessage, unsigned int aFlags)
        {
            char lMessage[LINE_LENGTH];

            Process_String(aMessage.c_str(), lMessage, sizeof(lMessage), aFlags | FLAG_MESSAGE);

            std::string lCmd = "echo ";

            lCmd += lMessage;

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        void Script_Sh::Write_Exit(int aCode)
        {
            std::string lCmd = "exit ";

            lCmd += std::to_string(aCode);

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        void Script_Sh::Write_If(const std::string& aCondition, unsigned int aFlags)
        {
            char lCondition[LINE_LENGTH];

            Process_String(aCondition.c_str(), lCondition, sizeof(lCondition), aFlags | FLAG_COMMAND);

            std::string lCmd = "if ";

            lCmd += lCondition;
            lCmd += " ; then";

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);

            Indent_Inc();
        }

        void Script_Sh::Write_If_End()
        {
            Script::Write_If_End();

            Write_Command("fi");
            Write_EmptyLine();
        }

        // NOT TESTED
        void Script_Sh::Write_If_Error() { Write_If("[ 0 != $? ]", FLAG_DO_NOT_PROCESS); }

        void Script_Sh::Write_If_NotExist(const char* aFileName, unsigned int aFlags)
        {
            char lFileName[PATH_LENGTH];

            Process_String(aFileName, lFileName, sizeof(lFileName), aFlags | FLAG_FILE_NAME);

            std::string lCond = "[ ! -e \"";

            lCond += lFileName;
            lCond += "\" ]";

            Write_If(lCond);
        }

        void Script_Sh::Write_Pause()
        {
            Write_Command("read -p \"Press Enter to continue...\"", FLAG_DO_NOT_PROCESS);
        }

        void Script_Sh::Write_Variable_Set(const char* aVariable, const char* aValue, unsigned int aFlags)
        {
            char lValue[LINE_LENGTH];

            Process_String(aValue, lValue, sizeof(lValue), aFlags);

            std::string lCmd = aVariable;

            lCmd += "=";
            lCmd += lValue;

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // ===== Script =====================================================

        // NORMAL <--+
        //  |        |
        //  +--> VARIABLE
        enum class State
        {
            NORMAL = 0,
            VARIABLE,
        };

        void Script_Sh::Process_String(const char* aIn, char* aOut, unsigned int aOutSize_byte, unsigned int aFlags)
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
                auto lFileName = 0 != (aFlags & FLAG_FILE_NAME);

                unsigned int lIn  = 0;
                unsigned int lOut = 0;

                auto lState = State::NORMAL;

                while (aOutSize_byte > lOut)
                {
                    switch (aIn[lIn])
                    {
                    case '\0':
                        aOut[lOut] = '\0'; lOut++;
                        return;

                    case '%':
                        switch (lState)
                        {
                        case State::NORMAL:
                            aOut[lOut] = '$'; lOut++;
                            lState = State::VARIABLE;
                            break;

                        case State::VARIABLE:
                            lState = State::NORMAL;
                            break;

                        default: assert(false);
                        }
                        break;

                    case '\\':
                        aOut[lOut] = lFileName ? '/' : aIn[lIn]; lOut++;
                        break;

                    default: aOut[lOut] = aIn[lIn]; lOut++;
                    }

                    lIn++;
                }

                KMS_EXCEPTION(RESULT_OUTPUT_TOO_SHORT, "String too long (NOT TESTED)", aIn);
            }
        }

        void Script_Sh::Write_Header()
        {
            Write_Line("#!/bin/sh");

            Script::Write_Header();
        }

    }
}
