
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Version.cpp

// TEST COVERAGE 2022-06-16 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/SafeAPI.h>
#include <KMS/Text/File_ASCII.h>

#include <KMS/Version.h>

KMS_RESULT_STATIC(RESULT_CORRUPTED_FILE);
KMS_RESULT_STATIC(RESULT_INVALID_NUMBER);

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    Version::Version() : mMajor(0), mMinor(0), mBuild(0), mCompat(0), mDebug(false)
    {
        Construct();
    }

    Version::Version(uint8_t aMajor, uint8_t aMinor, uint8_t aBuild, uint8_t aCompat, bool aDebug, const char * aDate, const char* aType)
        : mMajor(aMajor)
        , mMinor(aMinor)
        , mBuild(aBuild)
        , mCompat(aCompat)
        , mDebug(aDebug)
    {
        // assert(nullptr != aType);

        Construct();

        strncpy_s(mDate, aDate, sizeof(mDate) - 1);
        strncpy_s(mType, aType, sizeof(mType) - 1);
    }

    const char* Version::GetDate() const { return mDate; }
    const char* Version::GetType() const { return mType; }

    bool Version::GetDebug() const { return mDebug; }

    unsigned int Version::GetMajor () const { return mMajor ; }
    unsigned int Version::GetMinor () const { return mMinor ; }
    unsigned int Version::GetBuild () const { return mBuild ; }
    unsigned int Version::GetCompat() const { return mCompat; }

    #ifndef _KMS_EMBEDDED_

        Version::Version(const char* aVersion) : mDebug(false)
        {
            Construct();

            unsigned int lMajor;
            unsigned int lMinor = 0;
            unsigned int lBuild = 0;
            unsigned int lCompat = 0;

            if (   (5 != sscanf_s(aVersion, "%u.%u.%u.%u-%s", &lMajor, &lMinor, &lBuild, &lCompat, mType SizeInfo(mType)))
                && (4 != sscanf_s(aVersion, "%u.%u.%u-%s"   , &lMajor, &lMinor, &lBuild,           mType SizeInfo(mType)))
                && (4 != sscanf_s(aVersion, "%u.%u.%u.%u"   , &lMajor, &lMinor, &lBuild, &lCompat                       ))
                && (3 != sscanf_s(aVersion, "%u.%u-%s"      , &lMajor, &lMinor,                    mType SizeInfo(mType)))
                && (3 != sscanf_s(aVersion, "%u.%u.%u"      , &lMajor, &lMinor, &lBuild                                 ))
                && (2 != sscanf_s(aVersion, "%u.%u"         , &lMajor, &lMinor                                          )))
            {
                KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid version format", aVersion);
            }

            SetNumbers(lMajor, lMinor, lBuild, lCompat);
        }

        Version::Version(const File::Folder& aFolder, const char* aFile) : mDebug(false)
        {
            Construct();

            Text::File_ASCII lTF;

            lTF.Read(aFolder, aFile);

            lTF.RemoveEmptyLines();
            lTF.RemoveComments_CPP();

            unsigned int lState = 0;

            for (std::string lLine : lTF.mLines)
            {
                unsigned int lMajor;
                unsigned int lMinor;
                unsigned int lBuild;
                unsigned int lCompat;

                switch (lState)
                {
                case 0:
                    if (4 == sscanf_s(lLine.c_str(), "#define VERSION_RC %u,%u,%u,%u", &lMajor, &lMinor, &lBuild, &lCompat))
                    {
                        SetNumbers(lMajor, lMinor, lBuild, lCompat);
                        lState++;
                    }
                    break;
                case 1:
                    if (4 == sscanf_s(lLine.c_str(), "#define VERSION_STR \"%u.%u.%u.%u\"", &lMajor, &lMinor, &lBuild, &lCompat))
                    {
                        if (!Verify(lMajor, lMinor, lBuild, lCompat))
                        {
                            KMS_EXCEPTION(RESULT_CORRUPTED_FILE, "Incoherent version file (NOT TESTED)", aFile);
                        }
                        lState++;
                    }
                    break;
                case 2:
                    if (NULL != strstr(lLine.c_str(), "KMS_VERSION(\"\")"))
                    {
                        memset(&mType, 0, sizeof(mType));
                        return;
                    }
                    if (1 == sscanf_s(lLine.c_str(), " KMS_VERSION(\"%[^\"]\");", mType SizeInfo(mType)))
                    {
                        return;
                    }
                    break;

                default: assert(false);
                }
            }

            KMS_EXCEPTION(RESULT_CORRUPTED_FILE, "Incomplet version file (NOT TESTED)", aFile);
        }

        void Version::GetString(char* aOut, unsigned int aOutSize_byte, unsigned int aElement, bool aType) const
        {
            if (aType && (0 < strlen(mType)))
            {
                switch (aElement)
                {
                case 1: sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u-%s", mMajor, mType); break;
                case 2: sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u.%u-%s", mMajor, mMinor, mType); break;
                case 3: sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u.%u.%u-%s", mMajor, mMinor, mBuild, mType); break;
                case 4: sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u.%u.%u.%u-%s", mMajor, mMinor, mBuild, mCompat, mType); break;

                default: assert(false);
                }
            }
            else
            {
                switch (aElement)
                {
                case 1: sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u", mMajor); break;
                case 2: sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u.%u", mMajor, mMinor); break;
                case 3: sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u.%u.%u", mMajor, mMinor, mBuild); break;
                case 4: sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u.%u.%u.%u", mMajor, mMinor, mBuild, mCompat); break;

                default: assert(false);
                }
            }
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Version::Display(std::ostream& aOut) const
        {
            aOut << GetMajor() << "." << GetMinor() << "." << GetBuild();

            if (0 != mCompat)
            {
                aOut << "." << mCompat;
            }

            if (0 < strlen(mType))
            {
                aOut << "-" << mType;
            }

            if (0 < strlen(mDate))
            {
                aOut << " compiled on " << mDate;
            }

            if (mDebug)
            {
                aOut << " (debug)";
            }

            aOut << std::endl;
        }

    #endif

    // Private
    // //////////////////////////////////////////////////////////////////////

    void Version::Construct()
    {
        memset(&mDate, 0, sizeof(mDate));
        memset(&mType, 0, sizeof(mType));
    }

    bool Version::Verify(unsigned int aMajor, unsigned int aMinor, unsigned int aBuild, unsigned int aCompat) const
    {
        return (mMajor == aMajor) && (mMinor == aMinor) && (mBuild == aBuild) && (mCompat == aCompat);
    }

    #ifndef _KMS_EMBEDDED_

        void Version::SetNumbers(unsigned int aMajor, unsigned int aMinor, unsigned int aBuild, unsigned int aCompat)
        {
            KMS_EXCEPTION_ASSERT(256 > aMajor , RESULT_INVALID_NUMBER, "Invalid major number", aMajor);
            KMS_EXCEPTION_ASSERT(256 > aMinor , RESULT_INVALID_NUMBER, "Invalid minor number", aMinor);
            KMS_EXCEPTION_ASSERT(256 > aBuild , RESULT_INVALID_NUMBER, "Invalid build number", aBuild);
            KMS_EXCEPTION_ASSERT(256 > aCompat, RESULT_INVALID_NUMBER, "Invalid compatibility number", aCompat);

            mMajor  = aMajor;
            mMinor  = aMinor;
            mBuild  = aBuild;
            mCompat = aCompat;
        }

    #endif

}

using namespace KMS;

#ifndef _KMS_EMBEDDED_

    std::ostream& operator << (std::ostream& aOut, const Version& aV)
    {
        aV.Display(aOut);

        return aOut;
    }

#endif
