
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2026 KMS
/// \file      Includes/KMS/Script/Script.h

#pragma once

// ===== C++ ================================================================
#include <fstream>

namespace KMS
{
    namespace Script
    {

        // Brief Base class for script generator
        class Script
        {

        public:

            /// \brief This flag indicates the string is a command.
            static const unsigned int FLAG_COMMAND;

            /// \brief This flag indicates to not pause the script after
            ///        displaying the message.
            static const unsigned int FLAG_DO_NOT_PAUSE;

            /// \brief This flag indicates to not process the string in a any
            ///        way.
            static const unsigned int FLAG_DO_NOT_PROCESS;

            /// \brief This flag indicates to not quote the string (if
            ///        possible).
            static const unsigned int FLAG_DO_NOT_QUOTE;

            /// \brief This flag indicates the string is a file name.
            static const unsigned int FLAG_FILE_NAME;

            /// \brief This flag indicates the string is a message.
            static const unsigned int FLAG_MESSAGE;

            /// \brief This flag indicates to pause the script after
            ///        displaying the message.
            static const unsigned int FLAG_PAUSE;

            /// \brief Destructor
            virtual ~Script();

            /// \param aComment Write a comment with a section name into the
            ///                 script
            /// \see Write_Comment
            void Write_Comment_Section(const char* aComment);

            /// \param aFolder The folder name
            /// \param aFlags  FLAG_COMMAND and FLAG_FILE_NAME are assummed
            /// \see FLAG_DO_NOT_PROCESS
            void Write_CreateFolder_IfNeeded(const char* aFolder, unsigned int aFlags = 0);

            /// \param aFile  The file name
            /// \param aFlags FLAG_COMMAND and FLAG_FILE_NAME are assummed
            /// \see FLAG_DO_NOT_PROCESS
            void Write_DeleteFile_IfNeeded(const char* aFile, unsigned int aFlags = 0);

            /// \param aMessage Add a command to the script, to display this
            ///                 message
            /// \param aFlags   FLAG_MESSAGE is assumed
            /// \see FLAG_DO_NOT_PROCESS FLAG_DO_NOT_QUOTE, Write_Echo
            void Write_Echo_Executing(const char* aMessage, unsigned int aFlags = 0);

            /// \brief Add a command to the script, to display OK
            /// \see Write_Echo
            void Write_Echo_OK();

            /// \brief Add an empty line to the script
            void Write_EmptyLine();

            /// \param aMessage     The error message
            /// \param aInstruction Additionnal instruction the script
            ///                     display after the error message
            /// \param aFlags       FLAG_MESSAGE is assumed
            /// \see FLAG_DO_NOT_PAUSE, FLAG_DO_NO_PROCESS,
            ///      FLAG_DO_NOT_QUOTE, FLAG_PAUSE
            void Write_Error      (const char* aMessage, const char* aInstruction = nullptr, unsigned int aFlags = 0);
            void Write_Instruction(const char* aMessage, unsigned int aFlags = 0);
            void Write_Warning    (const char* aMessage, unsigned int aFlags = 0);

            /// \param aFile        The file name
            /// \param aInstruction Additionnal instruction the script
            ///                     display after the error message
            /// \param aFlags       FLAG_MESSAGE is assumed
            /// \see FLAG_DO_NOT_PAUSE, FLAG_DO_NOT_PROCESS,
            ///      FLAG_DO_NOT_QUOTE, Write_If_NotExist
            void Write_Verify_Exist(const char* aFile, const char* aInstruction = nullptr, unsigned int aFlags = 0);

            /// \param aMessage     The error message the script display on error
            /// \param aInstruction Additionnal instruction the script
            ///                     display after the error message
            /// \param aFlags       FLAG_MESSAGE is assumed
            /// \see FLAG_DO_NOT_PAUSE, FLAG_DO_NOT_PROCESS,
            ///      FLAG_DO_NOT_QUOTE, Write_If_Error
            void Write_Verify_Result(const char* aMessage, const char* aInstruction = nullptr, unsigned int aFlags = 0);

            /// \param aCommand The command to add to the script
            /// \param aFlags   FLAG_COMMAND is assumed
            /// \see FLAG_DO_NOT_PROCESS
            virtual void Write_Command(const std::string& aCommand, unsigned int aFlags = 0);

            /// \param aComment The comment to add to the script
            virtual void Write_Comment(const char* aComment) = 0;

            /// \param aFolder The folder name
            /// \param aFlags  FLAG_FILE_NAME is assummed
            /// \see FLAG_DO_NOT_PROCESS
            virtual void Write_CreateFolder(const char* aFolder, unsigned int aFlags) = 0;

            /// \param aFile  The file name
            /// \param aFlags FLAG_FILE_NAME is assummed
            /// \see FLAG_DO_NOT_PROCESS
            virtual void Write_DeleteFile(const char* aFile, unsigned int aFlags = 0) = 0;

            /// \param aMessage The message the command display
            /// \param aFlags   FLAG_MESSAGE is assumed
            /// \see FLAG_DO_NOT_PROCESS, FLAG_DO_NOT_QUOTE, FLAG_FILE_NAME
            virtual void Write_Echo(const std::string& aMessage, unsigned int aFlags = 0) = 0;

            /// \param aCode Add to the script a command exiting returning
            ///              this code
            virtual void Write_Exit(int aCode) = 0;

            /// \param aCondition The condition
            /// \param aFlags     FLAG_COMMAND is assumed
            /// \see FLAG_DO_NOT_PROCESS, FLAG_FILE_NAME, Write_If_End
            virtual void Write_If(const std::string& aCondition, unsigned int aFlags = 0) = 0;

            /// \brief Add the end of a if statement to the script
            /// \see Write_If
            virtual void Write_If_End();

            /// \brief Add to the script a if statement testing the result of
            ///        the last command
            /// \see Write_If_End
            virtual void Write_If_Error() = 0;

            /// \param aFile  The file name
            /// \param aFlags FLAG_COMMAND and FLAG_FILE_NAME are assumed
            /// \see FLAG_DO_NOT_PROCESS, FLAG_DO_NOT_QUOTE, Write_If_End
            virtual void Write_If_Exist(const char* aFile, unsigned int aFlags = 0) = 0;

            /// \param aFile  The file name
            /// \param aFlags FLAG_COMMAND and FLAG_FILE_NAME are assumed
            /// \see FLAG_DO_NOT_PROCESS, FLAG_DO_NOT_QUOTE, Write_If_End
            virtual void Write_If_NotExist(const char* aFile, unsigned int aFlags = 0) = 0;

            /// \brief Add a pause command into the script
            virtual void Write_Pause() = 0;

            /// \see FLAG_DO_NOT_PROCESS, Write_PushDirectory
            virtual void Write_PopDirectory() = 0;

            /// \param aFolder The folder name
            /// \param aFlags FLAG_COMMAND and FLAG_FILE_NAME are assumed
            /// \see FLAG_DO_NOT_PROCESS, Write_PopDirectory
            virtual void Write_PushDirectory(const char* aFolder, unsigned int aFlags = 0) = 0;

            /// \brief aVariable The variable name
            /// \brief aValue    The variable value
            /// \brief aFlags    FLAG_COMMAND_OUTPUT
            /// \see FLAG_DO_NOT_QUOTE, FLAG_FILE_NAME
            virtual void Write_Variable_Set(const char* aVariable, const char* aValue, unsigned int aFlags = 0) = 0;

        protected:

            /// \brief Constructor
            /// \param aFile      The file name, without the extension
            /// \param aExtension The file extension (including the initial dot)
            Script(const char* aFile, const char* aExtension);

            /// \param aLine The line to add to the script
            void Write_Line(const std::string& aLine);

            /// \brief Decrement indentation
            /// \see Indent_Inc
            virtual void Indent_Dec();

            /// \brief Increment indentation
            /// \see Indent_Inc
            virtual void Indent_Inc();

            /// \param aIn           Input string
            /// \param aOut          Output string
            /// \param aOutSize_byte Output string size
            /// \param aFlags        `FLAG_...`
            virtual void Process_String(const char* aIn, char* aOut, unsigned int aOutSize_byte, unsigned int aFlags) = 0;

            /// \brief Write script footer
            virtual void Write_Footer();

            /// \brief Write script header
            virtual void Write_Header();

        private:

            void Write_Pause(unsigned int aFlags, bool aDefault);

            int           mCode;
            std::string   mIndent;
            std::ofstream mOut;

        };

    }
}
