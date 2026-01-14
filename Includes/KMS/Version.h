
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

    class Version final
    {

    public:

        Version();

        Version(uint8_t aMajor, uint8_t aMinor, uint8_t aBuild, uint8_t aCompat, bool aDebug, const char * aDate, const char * aType);

        const char* GetDate() const;
        const char* GetType() const;

        bool GetDebug() const;

        unsigned int GetMajor() const;
        unsigned int GetMinor() const;
        unsigned int GetBuild() const;
        unsigned int GetCompat() const;

        bool IsZero() const;

        #ifndef _KMS_EMBEDDED_

            Version(const char* aVersion);

            Version(const File::Folder& aFolder, const char* aFile);

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
    std::ostream & operator << (std::ostream & aOut, const KMS::Version & aV);
#endif

#ifdef _DEBUG
    #define KMS_VERSION(T) static const KMS::Version VERSION(VERSION_RC, true, __DATE__, T);
#else
    #define KMS_VERSION(T) static const KMS::Version VERSION(VERSION_RC, false, __DATE__, T);
#endif
