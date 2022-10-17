
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Version.h

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

namespace KMS
{

    class Version
    {

    public:

        static const unsigned int FLAG_OS_INDEPENDENT;

        Version();

        Version(const char* aVersion);

        Version(const File::Folder& aFolder, const char * aFile);

        Version(uint8_t aMajor, uint8_t aMinor, uint8_t aBuild, uint8_t aCompat, bool aDebug, const char * aDate, const char * aType);

        const char* GetDate() const;
        const char* GetType() const;

        bool GetDebug() const;

        unsigned int GetMajor() const;
        unsigned int GetMinor() const;
        unsigned int GetBuild() const;
        unsigned int GetCompat() const;

        // aFlags FLAG_OS_INDEPENDENT
        void GetPackageName(const char* aProduct, char* aOut, unsigned int aOutSize_byte, unsigned int aFlags = 0) const;

        void GetString(char* aOut, unsigned int aOutSize_byte, unsigned int aElement = 3) const;

    private:

        void SetNumbers(unsigned int aMajor, unsigned int aMinor, unsigned int aBuild, unsigned int aCompat);

        bool Verify(unsigned int aMajor, unsigned int aMinor, unsigned int aBuild, unsigned int aCompat) const;

        uint8_t mMajor;
        uint8_t mMinor;
        uint8_t mBuild;
        uint8_t mCompat;

        bool mDebug;

        std::string mDate;
        std::string mType;

    };

}

std::ostream & operator << (std::ostream & aOut, const KMS::Version & aV);

#ifdef _DEBUG
    #define KMS_VERSION(T) static const KMS::Version VERSION(VERSION_RC, true, __DATE__, T);
#else
    #define KMS_VERSION(T) static const KMS::Version VERSION(VERSION_RC, false, __DATE__, T);
#endif
