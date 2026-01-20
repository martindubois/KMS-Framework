
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Script_Script_PS.cpp

// CODE REVIEW 2026-01-20 Martin Dubois
// TEST COVERAGE 2026-01-20 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Script/Script_PS.h>

namespace KMS
{
    namespace Script
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Script_PS::Script_PS(const char* aFileName) : Script(aFileName, ".ps1")
        {
            Write_Header();
        }

        // ===== Script =====================================================

        void Script_PS::Write_Comment(const char* aComment)
        {
            assert(nullptr != aComment);

            std::string lComment = "# ";

            lComment += aComment;

            Write_Line(lComment);
        }

        void Script_PS::Write_Echo(const std::string& aMessage, unsigned int aFlags)
        {
            char lMessage[LINE_LENGTH];

            Process_String(aMessage.c_str(), lMessage, sizeof(lMessage), aFlags | FLAG_MESSAGE);

            std::string lCmd = "Write-Output ( ";
            {
                lCmd += lMessage;
            }
            lCmd += " )";

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        void Script_PS::Write_Exit(int aCode)
        {
            std::string lCmd = "exit ";

            lCmd += std::to_string(aCode);

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        void Script_PS::Write_If(const std::string& aCondition, unsigned int aFlags)
        {
            char lCondition[LINE_LENGTH];

            Process_String(aCondition.c_str(), lCondition, sizeof(lCondition), aFlags | FLAG_COMMAND);

            std::string lCmd = "if ( ";
            {
                lCmd += aCondition;
            }
            lCmd += " )";

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);

            Indent_Inc();
        }

        void Script_PS::Write_If_Error() { Write_If("$?"); }

        void Script_PS::Write_If_NotExist(const char* aFileName, unsigned int aFlags)
        {
            char lFileName[PATH_LENGTH];

            Process_String(aFileName, lFileName, sizeof(lFileName), aFlags |= FLAG_FILE_NAME);

            std::string lCond = "! ( Test-Path ";
            {
                lCond += lFileName;
            }
            lCond += " )";

            Write_If(lCond, FLAG_DO_NOT_PROCESS);
        }

        void Script_PS::Write_Pause()
        {
            Write_Command("Read-Host -Prompt \"Press Enter to continue...\"", FLAG_DO_NOT_PROCESS);
        }

        void Script_PS::Write_Variable_Set(const char* aVariable, const char* aValue, unsigned int aFlags)
        {
            assert(nullptr != aVariable);
            assert(nullptr != aValue);

            char lValue[LINE_LENGTH];

            Process_String(aValue, lValue, sizeof(lValue), aFlags);

            std::string lCmd = "$";

            lCmd += aVariable;
            lCmd += " = ";
            lCmd += lValue;

            Write_Command(lCmd, FLAG_DO_NOT_PROCESS);
        }

        Script_PS::~Script_PS() { Write_Footer(); }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // ===== Script =====================================================

        void Script_PS::Indent_Dec()
        {
            Script::Indent_Dec();

            Write_Line("}");
            Write_EmptyLine();
        }

        void Script_PS::Indent_Inc()
        {
            Write_Line("{");

            Script::Indent_Inc();
        }

        // UNKNOWN <----------+
        //  |   |             |
        //  |   +--> COMMAND  |
        //  |           |     |
        //  +--> TEXT --+==> VARIABLE
        enum class State
        {
            UNKNOWN = 0,
            COMMAND,
            TEXT,
            VARIABLE,
        };

        void Script_PS::Process_String(const char* aIn, char* aOut, unsigned int aOutSize_byte, unsigned int aFlags)
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
                auto lMessage  = 0 != (aFlags & FLAG_MESSAGE);

                unsigned int lIn  = 0;
                unsigned int lOut = 0;

                auto lState = State::UNKNOWN;

                while (aOutSize_byte > lOut)
                {
                    switch (aIn[lIn])
                    {
                    case '\0':
                        switch (lState)
                        {
                        case State::TEXT:
                            aOut[lOut] = '\''; lOut++;
                            // no break

                        case State::COMMAND:
                        case State::UNKNOWN:
                            aOut[lOut] = '\0'; lOut++;
                            break;

                        default: KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid string format (NOT TESTED)", aIn);
                        }
                        return;

                    case '%':
                        switch (lState)
                        {
                        case State::COMMAND:
                        case State::UNKNOWN:
                            aOut[lOut] = '$'; lOut++;
                            lState = State::VARIABLE;
                            break;

                        case State::TEXT:
                            aOut[lOut] = '\''; lOut++;
                            aOut[lOut] = ' '; lOut++;
                            aOut[lOut] = '+'; lOut++;
                            aOut[lOut] = ' '; lOut++;
                            aOut[lOut] = '$'; lOut++;
                            lState = State::VARIABLE;
                            break;

                        case State::VARIABLE: lState = State::COMMAND; break;

                        default: assert(false);
                        }
                        break;

                    case '/':
                        switch (lState)
                        {
                        case State::COMMAND: aOut[lOut] = '/'; lOut++; break; // NOT TESTED

                        case State::TEXT: aOut[lOut] = lFileName ? '\\' : '/'; lOut++; break;

                        case State::UNKNOWN:
                            if (lFileName)
                            {
                                aOut[lOut] = '\''; lOut++;
                                aOut[lOut] = '\\'; lOut++;
                                lState = State::TEXT;
                            }
                            else if (lMessage)
                            {
                                aOut[lOut] = '\''; lOut++;
                                aOut[lOut] = '/'; lOut++;
                                lState = State::TEXT;
                            }
                            else
                            {
                                // NOT TESTED
                                aOut[lOut] = '/'; lOut++;
                                lState = State::COMMAND;
                            }
                            break;

                        default: KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid string (NOT TESTED)", aIn);
                        }
                        break;

                    default:
                        switch (lState)
                        {
                        case State::COMMAND:
                        case State::TEXT:
                        case State::VARIABLE:
                            aOut[lOut] = aIn[lIn]; lOut++;
                            break;

                        case State::UNKNOWN:
                            if (lFileName || lMessage)
                            {
                                aOut[lOut] = '\''; lOut++;
                                lState = State::TEXT;
                            }
                            else
                            {
                                lState = State::COMMAND;
                            }
                            aOut[lOut] = aIn[lIn]; lOut++;
                            break;
                        }
                        break;
                    }

                    lIn++;
                }

                KMS_EXCEPTION(RESULT_OUTPUT_TOO_SHORT, "String too long (NOT TESTED)", aIn);
            }
        }

    }
}
