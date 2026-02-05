
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2022-2026 KMS
/// \file      Includes/KMS/Version.h

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

namespace KMS
{

    /// \brief KMS software version
    /// \note This is what, here at KMS, we call a value class.
    ///       - copy allowed
    ///       - limited size
    ///       - no virtual method
    class Version final
    {

    public:

        /// \brief Default contructor
        Version();

        /// \brief Constructor
        /// \param aMajor  The major number
        /// \param aMinor  The minor number
        /// \param aBuild  The build number
        /// \param aCompat The compatibility number
        /// \param aDebug  false = Release, true = Debug
        /// \param aDate   The compilation date
        /// \param aType   The version type
        Version(uint8_t aMajor, uint8_t aMinor, uint8_t aBuild, uint8_t aCompat, bool aDebug, const char * aDate, const char * aType);

        /// \return The compilation date
        const char* GetDate() const;

        /// \return The version type
        const char* GetType() const;

        /// \retval false Release
        /// \retval true  Debug
        bool GetDebug() const;

        /// \return The major number
        unsigned int GetMajor() const;

        /// \return The minor number
        unsigned int GetMinor() const;

        /// \return The build number
        unsigned int GetBuild() const;

        /// \return The compatibility number
        unsigned int GetCompat() const;

        /// \retval false Valid version information
        /// \retval true  Empty version
        bool IsZero() const;

        #ifndef _KMS_EMBEDDED_

            /// \brief Constructor
            /// \param aVersion Version as a string
            Version(const char* aVersion);

            // TODO Remove use of File::Folder class by using a full path for
            //      the source file

            /// \brief Constructor
            /// \param aFolder The folder where the source file is
            /// \param aFile   The source file name
            Version(const File::Folder& aFolder, const char* aFile);

            /// \brief Retrieve version string
            /// \param aOut          The output buffer
            /// \param aOutSize_byte The output buffer size
            /// \param aElement      1 = Major, 2 = Major.Minor,
            ///                      3 = Major.Minor.Build,
            ///                      4 = Major.Minor.Build.Compat
            /// \param aType         false  Do not add type information
            ///                      true   Add type information
            void GetString(char* aOut, unsigned int aOutSize_byte, unsigned int aElement = 3, bool aType = false) const;

            // Internal

            void Display(std::ostream& aOut) const;

        #endif

    private:

        void Construct();

        bool Verify(unsigned int aMajor, unsigned int aMinor, unsigned int aBuild, unsigned int aCompat) const;

        #ifndef _KMS_EMBEDDED_
            void SetNumbers(unsigned int aMajor, unsigned int aMinor, unsigned int aBuild, unsigned int aCompat);
        #endif

        uint8_t mMajor;
        uint8_t mMinor;
        uint8_t mBuild;
        uint8_t mCompat;

        uint8_t mDebug;

        char mDate[27];
        char mType[16];

    };

}

#ifndef _KMS_EMBEDDED_
    /// \brief Output operator
    /// \param aOut The output stream
    /// \param aV   The version to display
    std::ostream & operator << (std::ostream & aOut, const KMS::Version & aV);
#endif

#ifdef _DEBUG
    /// \brief Create a static Version instance based on the `VERSION_RC`
    ///        constant
    /// \param T The version type
    #define KMS_VERSION(T) static const KMS::Version VERSION(VERSION_RC, true, __DATE__, T);
#else
    /// \brief Create a static Version instance based on the `VERSION_RC`
    ///        constant
    /// \param T The version type
    #define KMS_VERSION(T) static const KMS::Version VERSION(VERSION_RC, false, __DATE__, T);
#endif
