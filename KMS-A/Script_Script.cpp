
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Script_Script.cpp

// CODE REVIEW 2026-01-20 Martin Dubois
// TEST COVERAGE 2026-01-20 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Script/Script.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

static const char* EOL = "\n";

static const char* INDENT = "    ";

static constexpr unsigned int INDENT_LENGTH = 4;

namespace KMS
{
    namespace Script
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int Script::FLAG_COMMAND        = 0x00000001;
        const unsigned int Script::FLAG_DO_NOT_PAUSE   = 0x00000002;
        const unsigned int Script::FLAG_DO_NOT_PROCESS = 0x00000004;
        const unsigned int Script::FLAG_DO_NOT_QUOTE   = 0x00000008;
        const unsigned int Script::FLAG_FILE_NAME      = 0x00000010;
        const unsigned int Script::FLAG_MESSAGE        = 0x00000020;
        const unsigned int Script::FLAG_PAUSE          = 0x00000040;

        Script::~Script() {}

        void Script::Write_Comment_Section(const char* aComment)
        {
            assert(nullptr != aComment);

            std::string lComment = "===== ";

            lComment += aComment;
            lComment += " =====";

            Write_Comment(lComment.c_str());
        }

        void Script::Write_Echo_Executing(const char* aMessage, unsigned int aFlags)
        {
            assert(nullptr != aMessage);

            std::string lMsg = aMessage;

            lMsg += "...";

            Write_Echo(lMsg, aFlags);
            Write_EmptyLine();
        }

        void Script::Write_Echo_OK() { Write_Echo("OK", FLAG_DO_NOT_QUOTE); }

        void Script::Write_EmptyLine() { Write_Line(""); }

        void Script::Write_Error(const char* aMessage, const char* aInstruction, unsigned int aFlags)
        {
            assert(nullptr != aMessage);

            std::string lMsg = "ERROR  ";

            lMsg += aMessage;

            Write_Echo(lMsg);

            if (nullptr != aInstruction)
            {
                Write_Instruction(aInstruction);
            }

            Write_Pause(aFlags, true);
            Write_Exit(mCode);
            mCode += 10;
        }

        void Script::Write_Instruction(const char* aMessage, unsigned int aFlags)
        {
            assert(nullptr != aMessage);

            std::string lMsg = "INSTRUCTION  ";

            lMsg += aMessage;

            Write_Echo(lMsg);
            Write_Pause(aFlags, false);
        }

        void Script::Write_Warning(const char* aMessage, unsigned int aFlags)
        {
            assert(nullptr != aMessage);

            std::string lMsg = "WARNING  ";

            lMsg += aMessage;

            Write_Echo(lMsg);
            Write_Pause(aFlags, false);
        }

        void Script::Write_Verify_Exist(const char* aFileName, const char* aInstruction, unsigned int aFlags)
        {
            assert(nullptr != aFileName);

            Write_If_NotExist(aFileName);
            {
                std::string lMsg = aFileName;

                lMsg += "  does not exist";

                Write_Error(lMsg.c_str(), aInstruction, aFlags);
            }
            Write_If_End();
        }

        void Script::Write_Verify_Result(const char* aMessage, const char* aInstruction, unsigned int aFlags)
        {
            Write_If_Error();
            {
                Write_Error(aMessage, aInstruction, aFlags);
            }
            Write_If_End();
        }

        void Script::Write_Command(const std::string& aCommand, unsigned int aFlags)
        {
            char lCommand[LINE_LENGTH];

            Process_String(aCommand.c_str(), lCommand, sizeof(lCommand), aFlags | FLAG_COMMAND);

            Write_Line(aCommand);
        }

        void Script::Write_If_End() { Indent_Dec(); }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Script::Script(const char* aFileName, const char* aExtension)
            : mCode(10)
            , mIndent("")
            , mOut((std::string(aFileName) + aExtension).c_str(), std::ios_base::out | std::ios_base::trunc)
        {}

        void Script::Indent_Dec()
        {
            KMS_EXCEPTION_ASSERT(INDENT_LENGTH <= mIndent.size(), RESULT_INVALID_STATE, "Invalid indent", "");

            for (unsigned int i = 0; i < INDENT_LENGTH; i++)
            {
                mIndent.pop_back();
            }
        }

        void Script::Indent_Inc() { mIndent += "    "; }

        void Script::Write_Line(const std::string& aLine) { mOut << mIndent << aLine << EOL; }

        void Script::Write_Footer()
        {
            Write_EmptyLine();
            Write_Comment_Section("End");
            Write_Echo_OK();
        }

        void Script::Write_Header()
        {
            Write_EmptyLine();
            Write_Comment("This file has been genereted using KMS-Framework");
            Write_EmptyLine();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Script::Write_Pause(unsigned int aFlags, bool aDefault)
        {
            bool lPause = aDefault;

            if (0 != (aFlags & FLAG_DO_NOT_PAUSE))
            {
                lPause = false;
            }

            if (0 != (aFlags & FLAG_PAUSE))
            {
                lPause = true;
            }

            if (lPause)
            {
                Write_Pause();
            }
        }

    }
}
