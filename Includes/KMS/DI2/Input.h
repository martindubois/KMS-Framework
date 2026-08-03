
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2025-2026 KMS
/// \file      Includes/KMS/DI2/Input.h

#pragma once

// ===== C++ ================================================================
#include <regex>

// ===== Includes ===========================================================
#include <KMS/ArgList.h>
#include <KMS/DI2/Operator.h>
#include <KMS/DI2/TokenType.h>

namespace KMS
{
    namespace DI2
    {

        class IType;

        /// \brief Input data to the `DI2` infrastructure
        /// \note This class is not intended to be used by KMS-Framework's
        ///       users. The normal KMS-Framework users should use the
        ///       `Decode_...` functions
        class Input final
        {

        public:

            /// \brief Constructor
            Input();

            /// \brief Destructor
            ~Input();

            /// \param aFile File to parse
            /// \exception KMS::Exception RESULT_OPEN_FAILED
            ///                           RESULT_OPERATION_FILE
            ///                           RESULT_READ_FAILED
            void Init_File(const char* aFile);

            /// \param aString The string to parse
            void Init_String(const char* aString);

            /// \retval false Not consummed
            /// \retval true  Consummed
            bool IsConsummed();

            /// \param aRegex The regular expression
            void Next(const std::regex& aRegex);

            /// \param aRegex The regular expression
            /// \param aMatch The match container
            void Next(const std::regex& aRegex, std::smatch& aMatch);

            /// \param aRegex The regular expression
            /// \retval false  No match
            /// \retval true   Match
            bool Next_Try(const std::regex& aRegex);

            /// \param aRegex The regular expression
            /// \param aMatch The match container
            /// \retval false  No match
            /// \retval true   Match
            bool Next_Try(const std::regex& aRegex, std::smatch& aMatch);

        private:

            void SkipBlank();

            bool         mDelete;
            unsigned int mIndex;
            const char * mString;
            std::string  mStringR;
            char         mValue[LINE_LENGTH];

        };

        /// \param aData   Data output
        /// \param aType   The `DI2` type description
        /// \param aCount  The argument count
        /// \param aVector The arguments to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_ASCII_Arguments(void* aData, const IType* aType, int aCount, const char** aVector);

        /// \param aData   Data output
        /// \param aType   The `DI2` type description
        /// \param aArgList The `ArgList` to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_ASCII_Arguments(void* aData, const IType* aType, KMS::ArgList* aArgList);

        /// \param aData    Data output
        /// \param aType    The `DI2` type description
        /// \param aVarName The environment variable name
        void Decode_ASCII_Env(void* aData, const IType* aType, const char* aVarName);

        /// \param aData Data output
        /// \param aType The `DI2` type description
        /// \param aFile The file to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        ///                           RESULT_OPEN_FAILED
        ///                           RESULT_OPERATION_FILE
        ///                           RESULT_READ_FAILED
        void Decode_ASCII_File(void* aData, const IType* aType, const char* aFile);

        /// \param aData   Data output
        /// \param aType   The `DI2` type description
        /// \param aString The string to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_ASCII_String(void* aData, const IType* aType, const char* aString);

        /// \param aData   Data output
        /// \param aType   The `DI2` type description
        /// \param aString The string to try to parse
        /// \retval false Cannot parse the string
        /// \retval true  String parsed
        bool Decode_ASCII_String_Try(void* aData, const IType* aType, const char* aString);

        /// \param aData  Data output
        /// \param aType  The `DI2` type description
        /// \param aInput The input to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_JSON(void* aData, const IType* aType, Input* aInput);

        /// \param aData   Data output
        /// \param aType   The `DI2` type description
        /// \param aString The string to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_JSON_String(void* aData, const IType* aType, const char* aString);

    }
}
