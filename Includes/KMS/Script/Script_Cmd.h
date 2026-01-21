
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2026 KMS
/// \file      Includes/KMS/Script/Script_Cmd.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Script/Script.h>

namespace KMS
{
    namespace Script
    {

        /// \brief Class used to generate batch file
        class Script_Cmd final : public Script
        {

        public:

            /// \brief Constructor
            /// \param aFile The name of the script without the extension
            Script_Cmd(const char* aFile);

            // ===== Script =================================================
            virtual ~Script_Cmd() override;
            virtual void Write_Comment(const char* aComment) override;
            virtual void Write_CreateFolder(const char* aFolder, unsigned int aFlags = 0) override;
            virtual void Write_DeleteFile(const char* aFile, unsigned int aFlags = 0) override;
            virtual void Write_Echo(const std::string& aMessage, unsigned int aFlags = 0) override;
            virtual void Write_Exit(int aCode) override;
            virtual void Write_If(const std::string& aCondition, unsigned int aFlags = 0) override;
            virtual void Write_If_Error() override;
            virtual void Write_If_Exist(const char* aFile, unsigned int aFlags = 0) override;
            virtual void Write_If_NotExist(const char* aFile, unsigned int aFlags = 0) override;
            virtual void Write_Pause() override;
            virtual void Write_PopDirectory() override;
            virtual void Write_PushDirectory(const char* aFolder, unsigned int aFlags = 0) override;
            virtual void Write_Variable_Set(const char* aVariable, const char* aValue, unsigned int aFlags = 0) override;

        protected:
            // ===== Script =================================================
            virtual void Indent_Dec() override;
            virtual void Process_String(const char* aIn, char* aOut, unsigned int aOutSize_byte, unsigned int aFlags) override;
            virtual void Write_Footer() override;
            virtual void Write_Header() override;

        };

    }
}
