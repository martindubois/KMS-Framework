
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2023-2026 KMS
/// \file      Includes/KMS/Build/Package.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Version.h>

namespace KMS
{
    namespace Build
    {

        /// \brief Package name
        class Package final
        {

        public:

            /// \brief Default contructor
            /// \param aOSIndependent false = OS dependent, true = OS
            ///                       independent
            Package(bool aOSIndependent = false);

            /// \brief Retrieve the file name
            /// \param aOut          Output buffer
            /// \param aOutSize_byte Output buffer size
            void GetFileName(char* aOut, unsigned int aOutSize_byte) const;

            /// \brief Retrieve the product name
            /// \return The product name
            const char* GetProductName() const;

            /// \brief Retrieve the part of name including version and OS
            /// \param aOut          Output buffer
            /// \param aOutSize_byte Output buffer size
            void GetTagName(char* aOut, unsigned int aOutSize_byte) const;

            /// \brief Retrieve the version
            /// \return The version
            const Version& GetVersion() const;

            /// \brief Set the product name and the version
            /// \param aProduct The product name
            /// \param aVersion The version
            void Set(const char* aProduct, const Version& aVersion);

            /// \brief Parse a file name
            /// \param aIn The package name to parse
            void Parse(const char* aIn);

        private:

            bool         mOSIndependent;
            String_ASCII mProduct;
            Version      mVersion;

        };

    }
}
