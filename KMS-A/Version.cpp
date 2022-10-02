
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Version.cpp

// TEST COVERAGE 2022-06-16 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/SafeAPI.h>
#include <KMS/Version.h>
#include <KMS/Text/File_ASCII.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    const unsigned int Version::FLAG_OS_INDEPENDENT = 0x00000001;

    Version::Version(const char* aVersion)
    {
        unsigned int lMajor;
        unsigned int lMinor = 0;
        unsigned int lBuild = 0;
        unsigned int lCompat = 0;

        char lType[NAME_LENGTH];

        memset(&lType, 0, sizeof(lType));

        if (   (5 != sscanf_s(aVersion, "%u.%u.%u.%u-%s", &lMajor, &lMinor, &lBuild, &lCompat, lType SizeInfo(lType)))
            && (4 != sscanf_s(aVersion, "%u.%u.%u-%s"   , &lMajor, &lMinor, &lBuild,           lType SizeInfo(lType)))
            && (4 != sscanf_s(aVersion, "%u.%u.%u.%u"   , &lMajor, &lMinor, &lBuild, &lCompat                       ))
            && (3 != sscanf_s(aVersion, "%u.%u-%s"      , &lMajor, &lMinor,                    lType SizeInfo(lType)))
            && (3 != sscanf_s(aVersion, "%u.%u.%u"      , &lMajor, &lMinor, &lBuild                                 ))
            && (2 != sscanf_s(aVersion, "%u.%u"         , &lMajor, &lMinor                                          )))
        {
            KMS_EXCEPTION(VERSION_FORMAT, "Invalid version format", aVersion);
        }

        SetNumbers(lMajor, lMinor, lBuild, lCompat);

        mType = lType;
    }

    Version::Version(const File::Folder& aFolder, const char* aFile)
    {
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
                        // NOT TESTED
                        KMS_EXCEPTION(VERSION_FILE, "Incoherent version file", aFile);
                    }
                    lState++;
                }
                break;
            case 2:
                if (NULL != strstr(lLine.c_str(), "KMS_VERSION(\"\")"))
                {
                    mType = "";
                    return;
                }
                char lType[32];
                if (1 == sscanf_s(lLine.c_str(), " KMS_VERSION(\"%[^\"]\");", lType SizeInfo(lType)))
                {
                    mType = lType;
                    return;
                }
                break;

            default: assert(false);
            }
        }

        // NOT TESTED
        KMS_EXCEPTION(VERSION_FILE, "Incomplet version file", aFile);
    }

    Version::Version(uint8_t aMajor, uint8_t aMinor, uint8_t aBuild, uint8_t aCompat, bool aDebug, const char * aDate, const char* aType)
        : mMajor(aMajor)
        , mMinor(aMinor)
        , mBuild(aBuild)
        , mCompat(aCompat)
        , mDebug(aDebug)
        , mDate(aDate)
        , mType(aType)
    {
        assert(NULL != aType);
    }

    const char* Version::GetDate() const { return mDate.c_str(); }
    const char* Version::GetType() const { return mType.c_str(); }

    bool Version::GetDebug() const { return mDebug; }

    unsigned int Version::GetMajor () const { return mMajor ; }
    unsigned int Version::GetMinor () const { return mMinor ; }
    unsigned int Version::GetBuild () const { return mBuild ; }
    unsigned int Version::GetCompat() const { return mCompat; }

    void Version::GetPackageName(const char* aProduct, char* aOut, unsigned int aOutSize_byte, unsigned int aFlags) const
    {
        assert(NULL != aProduct);
        assert(NULL != aOut);
        assert(0 < aOutSize_byte);

        if (0 == (aFlags & FLAG_OS_INDEPENDENT))
        {
            if (0 < mType.size())
            {
                sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s_%s_%u.%u.%u-%s.zip", aProduct, OS_NAME_PROC, mMajor, mMinor, mBuild, mType.c_str());
            }
            else
            {
                sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s_%s_%u.%u.%u.zip", aProduct, OS_NAME_PROC, mMajor, mMinor, mBuild);
            }
        }
        else
        {
            if (0 < mType.size())
            {
                sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s_%u.%u.%u-%s.zip", aProduct, mMajor, mMinor, mBuild, mType.c_str());
            }
            else
            {
                sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s_%u.%u.%u.zip", aProduct, mMajor, mMinor, mBuild);
            }
        }
    }

    void Version::GetString(char* aOut, unsigned int aOutSize_byte, unsigned int aElement) const
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

    // Private
    // //////////////////////////////////////////////////////////////////////

    void Version::SetNumbers(unsigned int aMajor, unsigned int aMinor, unsigned int aBuild, unsigned int aCompat)
    {
        KMS_EXCEPTION_ASSERT(256 > aMajor , VERSION_NUMBER, "Invalid major number", aMajor);
        KMS_EXCEPTION_ASSERT(256 > aMinor , VERSION_NUMBER, "Invalid minor number", aMinor);
        KMS_EXCEPTION_ASSERT(256 > aBuild , VERSION_NUMBER, "Invalid build number", aBuild);
        KMS_EXCEPTION_ASSERT(256 > aCompat, VERSION_NUMBER, "Invalid compatibility number", aCompat);

        mMajor = aMajor;
        mMinor = aMinor;
        mBuild = aBuild;
        mCompat = aCompat;
    }

    bool Version::Verify(unsigned int aMajor, unsigned int aMinor, unsigned int aBuild, unsigned int aCompat) const
    {
        return (mMajor == aMajor) && (mMinor == aMinor) && (mBuild == aBuild) && (mCompat == aCompat);
    }

}

std::ostream& operator << (std::ostream& aOut, const KMS::Version& aV)
{
    aOut << aV.GetMajor() << "." << aV.GetMinor() << "." << aV.GetBuild() << "." << aV.GetCompat() << " " << aV.GetType();
    
    const char* lDate = aV.GetDate();
    if (0 < strlen(lDate))
    {
        aOut << " compiled on " << lDate;
    }

    if (aV.GetDebug())
    {
        aOut << " (debug)";
    }

    aOut << std::endl;

    return aOut;
}
