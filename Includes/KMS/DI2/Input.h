
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2025-2026 KMS
/// \file      Includes/KMS/DI2/Input.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/ArgList.h>
#include <KMS/DI2/Operator.h>
#include <KMS/DI2/TokenType.h>

namespace KMS
{
    namespace DI2
    {

        class IType;

        /// \brief Input data to the DI2 infrastructure
        /// \note This class is not intended to be used by KMS-Framework's
        ///       users. The normal KMS-Framework users should use the
        ///       Decode_... functions
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

            /// \brief Go back one token or one char
            /// \see Char_Next Char_Next_Try Token_Next
            void Previous();

            // ===== Char ===================================================

            /// \param aChar Verify the next char is this one
            /// \see Previous
            /// \exception KMS::Exception RESULT_INVALID_FORMAT
            void Char_Next(char aChar);

            /// \param aChars Verify the next char is one of them
            /// \return The current char
            /// \exception KMS::Exception RESULT_INVALID_FORMAT
            /// \see Previous
            char Char_Next(const char* aChars);

            /// \param aChar Verify the next char is this one
            /// \retval false This is not the next char
            /// \retval true  This is the next char
            /// \see Previous
            bool Char_Next_Try(char aChar);

            // ===== Token ==================================================

            /// \param aTypes Allower token types
            /// \return The current token type
            /// \exception KMS::Exception RESULT_INVALID_FORMAT
            /// \see Previous
            TokenType Token_Next(TokenType aTypes);

            /// \return The double value in the current token (FLOAT)
            /// \exception KMS::Exception RESULT_INVALID_FORMAT
            double Token_GetFloat() const;

            /// \return The int64_t value in the current token (INT)
            /// \exception KMS::Exception RESULT_INVALID_FORMAT
            int64_t Token_GetInt() const;

            /// \return The operator in the current token (OPERATOR)
            /// \exception KMS::Exception RESULT_INVALID_FORMAT
            Operator Token_GetOperator() const;

            /// \param aOut          The output buffer
            /// \param aOutSize_byte The output buffer size
            /// \exception KMS::Exception RESULT_OUTPUT_TOO_SHORT
            void Token_GetText(char* aOut, unsigned int aOutSize_byte) const;

            /// \return The uint64_t in the current token (UINT, UINT_HEX)
            /// \exception KMS::Exception RESULT_INVALID_FORMAT
            uint64_t Token_GetUInt() const;

        private:

            void SkipBlank();

            bool         mDelete;
            unsigned int mIndex;
            unsigned int mPrevious;
            const char * mString;
            char         mValue[LINE_LENGTH];

        };

        /// \param aData   Data output
        /// \param aType   The type description
        /// \param aCount  The argument count
        /// \param aVector The arguments to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_ASCII_Arguments(void* aData, const IType* aType, int aCount, const char** aVector);

        /// \param aData   Data output
        /// \param aType   The type description
        /// \param aArgList The ArgList to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_ASCII_Arguments(void* aData, const IType* aType, KMS::ArgList* aArgList);

        /// \param aData Data output
        /// \param aType The type description
        /// \param aFile The file to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        ///                           RESULT_OPEN_FAILED
        ///                           RESULT_OPERATION_FILE
        ///                           RESULT_READ_FAILED
        void Decode_ASCII_File(void* aData, const IType* aType, const char* aFile);

        /// \param aData   Data output
        /// \param aType   The type description
        /// \param aString The string to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_ASCII_String(void* aData, const IType* aType, const char* aString);

        /// \param aData   Data output
        /// \param aType   The type description
        /// \param aString The string to try to parse
        /// \retval false Cannot parse the string
        /// \retval true  String parsed
        bool Decode_ASCII_String_Try(void* aData, const IType* aType, const char* aString);

        /// \param aData  Data output
        /// \param aType  The type description
        /// \param aInput The input to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_JSON(void* aData, const IType* aType, Input* aInput);

        /// \param aData   Data output
        /// \param aType   The type description
        /// \param aString The string to parse
        /// \exception KMS::Exception RESULT_INVALID_FORMAT
        void Decode_JSON_String(void* aData, const IType* aType, const char* aString);

    }
}
