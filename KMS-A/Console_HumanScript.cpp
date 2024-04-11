
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Console_HumanScript.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>

#include <KMS/Console/HumanScript.h>

// Configurtions
// //////////////////////////////////////////////////////////////////////////

#define CHECK   "[ ]\n"
#define CHECK_X (75)

namespace KMS
{
    namespace Console
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char HumanScript::NO_DEFAULT = '\0';

        const char HumanScript::RESULT_ABORT = '\001';

        const char HumanScript::RESULT_ERROR = 'e';
        const char HumanScript::RESULT_NO    = 'n';
        const char HumanScript::RESULT_OK    = 'o';

        const char* HumanScript::RESULTS_ERROR_NO_OK = "eno";
        const char* HumanScript::RESULTS_ERROR_OK    = "eo";
        const char* HumanScript::RESULTS_NO_OK       = "no";

        HumanScript::HumanScript() : mIndent(0) {}

        void HumanScript::Begin(const char* aTitle)
        {
            assert(nullptr != aTitle);

            mIndent = 0;

            std::cout << Console::Color::BLUE << "INSTRUCTION  " << aTitle << "\n";
        }

        void HumanScript::End()
        {
            mIndent = 0;

            std::cout << Console::Color::WHITE << std::endl;
        }

        void HumanScript::Line(const char* aLine)
        {
            assert(nullptr != aLine);

            Indent();

            std::cout << aLine << "\n";
        }

        char HumanScript::Menu(char aDefault, const char* aResponse)
        {
            assert(nullptr != aResponse);

            char lResult;

            for (;;)
            {
                std::cout << "Enter your choice (Default = " << aDefault << ") : " << std::flush;

                char lLine[LINE_LENGTH];

                auto lRet = gets_s(lLine);
                if (nullptr == lRet)
                {
                    lResult = RESULT_ABORT;
                    break;
                }

                lResult = lLine[0];

                if ('\0' == lResult)
                {
                    lResult = aDefault;
                }

                if (nullptr != strchr(aResponse, lResult))
                {
                    break;
                }

                std::cout << "Invalid choice" << std::endl;
            }

            return lResult;
        }

        void HumanScript::Step(const char* aStep)
        {
            assert(nullptr != aStep);

            auto lX = Indent();

            std::cout << aStep;

            lX += static_cast<unsigned int>(strlen(aStep));

            if (CHECK_X <= lX)
            {
                std::cout << "\n";
                lX = 0;
            }

            Spaces(CHECK_X - lX);
            std::cout << CHECK;
        }

        void HumanScript::Wait()
        {
            mIndent = 0;

            std::cout << "Press ENTER to continue" << std::endl;

            char lLine[LINE_LENGTH];

            auto lRet = gets_s(lLine);
            KMS_EXCEPTION_ASSERT(nullptr != lRet, RESULT_READ_FAILED, "gets_s failed", "");
        }

        char HumanScript::Wait(char aDefault, const char* aResponses)
        {
            assert(nullptr != aResponses);

            if (nullptr != strchr(aResponses, 'e')) { std::cout << "  e.  An error occured\n"; }
            if (nullptr != strchr(aResponses, 'n')) { std::cout << "  n.  Not exectuted\n"; }
            if (nullptr != strchr(aResponses, 'o')) { std::cout << "  o.  Exectuted successfully\n"; }

            return Menu(aDefault, aResponses);
        }

        void HumanScript::Indent_Dec()
        {
            assert(0 < mIndent);

            mIndent--;
        }

        void HumanScript::Indent_Inc() { mIndent++; }

        void HumanScript::Indent_Reset() { mIndent = 0; }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int HumanScript::Indent()
        {
            unsigned int lResult = 0;

            for (unsigned int i = 0; i < mIndent; i++)
            {
                std::cout << "    ";
                lResult += 4;
            }

            return lResult;
        }

        void HumanScript::Spaces(unsigned int aCount)
        {
            for (unsigned int i = 0; i < aCount; i++)
            {
                std::cout << " ";
            }
        }

    }
}
