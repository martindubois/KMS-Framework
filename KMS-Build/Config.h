
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Config.h

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/Version.h>

class Config
{

public:

    #ifdef _KMS_LINUX_
        Config(bool aDoNotClean, bool aDoNotCompile, bool aDoNotExport, bool aDoNotPackage, bool aDoNotTest,
            bool aEmbedded, const KMS::File::Folder aExportFolder, bool aOSIndependent,
            const char* aProduct, const KMS::Version& aVersion);
    #endif

    #ifdef _KMS_WINDOWS_
        Config(const char* aCertificatSHA1, bool aDoNotClean, bool aDoNotCompile, bool aDoNotExport, bool aDoNotPackage, bool aDoNotTest,
            bool aEmbedded, const KMS::File::Folder aExportFolder, bool aOSIndependent,
            const char* aProduct, const KMS::Version& aVersion, uint32_t aVisualStudioVersion);
    #endif

    bool GetDoNotClean  () const;
    bool GetDoNotCompile() const;
    bool GetDoNotExport () const;
    bool GetDoNotPackage() const;
    bool GetDoNotTest   () const;

    const KMS::File::Folder& GetExportFolder() const;

    const char* GetProduct() const;

    KMS::File::Folder* GetProductFolder() const;

    const KMS::Version& GetVersion() const;

    bool IsEmbedded() const;

    bool IsOSIndependent() const;

    #ifdef _KMS_WINDOWS_
        const char* GetCertificatSHA1     () const;
        uint32_t    GetVisualStudioVersion() const;
    #endif

private:

    bool              mDoNotClean;
    bool              mDoNotCompile;
    bool              mDoNotExport;
    bool              mDoNotPackage;
    bool              mDoNotTest;
    bool              mEmbedded;
    KMS::File::Folder mExportFolder;
    bool              mOSIndependent;
    std::string       mProduct;
    KMS::Version      mVersion;

    #ifdef _KMS_WINDOWS_
        std::string mCertificatSHA1;
        uint32_t    mVisualStudioVersion;
    #endif

};
