
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Console/HumanScript.h
// Library   KMS-A

#pragma once

namespace KMS
{
    namespace Console
    {

        class HumanScript final
        {

        public:

            static const char NO_DEFAULT;

            static const char RESULT_ABORT;
            static const char RESULT_ERROR;
            static const char RESULT_NO;
            static const char RESULT_OK;

            static const char* RESULTS_ERROR_OK;
            static const char* RESULTS_ERROR_NO_OK;
            static const char* RESULTS_NO_OK;

            HumanScript();

            void Begin(const char* aTitle);
            void End  ();
            void Line (const char* aLine);
            char Menu (char aDefault, const char* aResponses);
            void Step (const char* aStep);

            void Wait();
            char Wait(char aDefault, const char* aResponse);

            void Indent_Dec();
            void Indent_Inc();
            void Indent_Reset();

        private:

            unsigned int Indent();

            void Spaces(unsigned int aCount);

            unsigned int mIndent;

        };

    }
}
