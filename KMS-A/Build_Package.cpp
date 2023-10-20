
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Package.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Build/Package.h>

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Package::Package(bool aOSIndependent) : mOSIndependent(aOSIndependent) {}

        void Package::GetFileName(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);

            auto lProduct = mProduct.c_str();

            auto lMajor = mVersion.GetMajor();
            auto lMinor = mVersion.GetMinor();
            auto lBuild = mVersion.GetBuild();
            auto lType  = mVersion.GetType ();

            auto lTypePresent = 0 < strlen(lType);

            if (mOSIndependent)
            {
                if (lTypePresent)
                {
                    sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s_%u.%u.%u-%s.zip", lProduct, lMajor, lMinor, lBuild, lType);
                }
                else
                {
                    sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s_%u.%u.%u.zip", lProduct, lMajor, lMinor, lBuild);
                }
            }
            else
            {
                if (lTypePresent)
                {
                    sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s_%s_%u.%u.%u-%s.zip", lProduct, OS_NAME_PROC, lMajor, lMinor, lBuild, lType);
                }
                else
                {
                    sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s_%s_%u.%u.%u.zip", lProduct, OS_NAME_PROC, lMajor, lMinor, lBuild);
                }
            }
        }

        const char* Package::GetProductName() const { return mProduct.c_str(); }

        void Package::GetTagName(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);

            auto lMajor = mVersion.GetMajor();
            auto lMinor = mVersion.GetMinor();
            auto lBuild = mVersion.GetBuild();
            auto lType  = mVersion.GetType ();

            auto lTypePresent = 0 < strlen(lType);

            if (mOSIndependent)
            {
                if (lTypePresent)
                {
                    sprintf_s(aOut SizeInfo(aOutSize_byte), "%u.%u.%u-%s", lMajor, lMinor, lBuild, lType);
                }
                else
                {
                    sprintf_s(aOut SizeInfo(aOutSize_byte), "%u.%u.%u", lMajor, lMinor, lBuild);
                }
            }
            else
            {
                if (lTypePresent)
                {
                    sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u.%u.%u-%s_%s", lMajor, lMinor, lBuild, lType, OS_NAME_PROC);
                }
                else
                {
                    sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u.%u.%u_%s", lMajor, lMinor, lBuild, OS_NAME_PROC);
                }
            }
        }

        const Version& Package::GetVersion() const { return mVersion; }

        void Package::Set(const char* aProduct, const Version& aVersion)
        {
            assert(nullptr != aProduct);

            mProduct = aProduct;
            mVersion = aVersion;
        }

        void Package::Parse(const char* aIn)
        {
            assert(nullptr != aIn);

            char lProduct[NAME_LENGTH];
            char lVersion[NAME_LENGTH];

            auto lRet = sscanf_s(aIn, "%[^;] ; %[^\n\r]", lProduct SizeInfo(lProduct), lVersion SizeInfo(lVersion));
            KMS_EXCEPTION_ASSERT(2 == lRet, RESULT_INVALID_FORMAT, "Invalid dependency format", aIn);

            mProduct = lProduct;
            mVersion = Version(lVersion);
        }

    }
}
