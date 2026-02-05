
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2024-2026 KMS
/// \file      Includes/KMS/Console/HumanScript.h

#pragma once

namespace KMS
{
    namespace Console
    {

        /// \brief Class to ouput instruction to the terminal
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

            /// \brief Default constructor
            HumanScript();

            /// \brief Begin instruction
            /// \param aTitle The title
            /// \see End
            void Begin(const char* aTitle);

            /// \brief End instruction
            /// \see Begin
            void End();

            /// \brief Output a line of instruction
            /// \param aLine The line
            void Line(const char* aLine);

            /// \brief Wait for response
            /// \param aDefault  The default response. See `RESULT_...`
            /// \param aResponse Possible responses. See `RESULTS_...`
            /// \retval RESULT_ABORT
            /// \retval RESULT_ERROR
            /// \retval RESULT_NO
            /// \retval RESULT_OK
            /// \see Wait
            char Menu(char aDefault, const char* aResponses);

            /// \brief Display a instruction step
            /// \param aStep The step
            void Step(const char* aStep);

            /// \brief Wait until the user press ENTER
            void Wait();

            /// \brief Display menu and wait for a user response
            /// \param aDefault   The default respone. See `RESULT_...`
            /// \param aResponses Possible responses. See `RESULTS_...`
            /// \see Menu
            char Wait(char aDefault, const char* aResponses);

            /// \brief Decrement indentation
            void Indent_Dec();

            /// \brief Increment indentation
            void Indent_Inc();

            /// \brief Reset indentation
            void Indent_Reset();

        private:

            unsigned int Indent();

            void Spaces(unsigned int aCount);

            unsigned int mIndent;

        };

    }
}
