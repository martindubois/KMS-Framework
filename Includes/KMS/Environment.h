
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// Status    PROD_READY (Windows)
// Library   KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2022-2026 KMS
/// \file      Includes/KMS/Environment.h

#pragma once

namespace KMS
{
    namespace Env
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        /// \brief Expand environment variables
        /// \param aIn           Input string
        /// \param aOut          Output string
        /// \param aOutSize_byte The output string size
        /// \exception KMS::Exception RESULT_EXPAND_FAILED
        ///                           RESULT_OUTPUT_TOO_SHORT
        void Expand(const char* aIn, char* aOut, unsigned int aOutSize_byte);

        /// \brief Retrieve the value of an environment variable
        /// \param aName         Variable name
        /// \param aOut          Output string
        /// \param aOutSize_byte The output string size
        /// \exception KMS::Exception RESULT_EXPAND_FAILED
        ///                           RESULT_OUTPUT_TOO_SHORT
        unsigned int  GetVariableValue(const char* aName, char* aOut, unsigned int aOutSize_byte);

    }
}
