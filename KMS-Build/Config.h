
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Config.h

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/Version.h>

class Config
{

public:

    Config(bool aDoNotClean, bool aDoNotCompile, bool aDoNotExport, bool aDoNotPackage, bool aDoNotTest,
        bool aEmbedded, const KMS::File::Folder aExportFolder, bool aOSIndependent,
        const char* aProduct, const char* aReadMePrefix, const KMS::Version& aVersion);

    bool GetDoNotClean  () const;
    bool GetDoNotCompile() const;
    bool GetDoNotExport () const;
    bool GetDoNotPackage() const;
    bool GetDoNotTest   () const;

    const KMS::File::Folder& GetExportFolder() const;

    const char* GetProduct() const;

    KMS::File::Folder* GetProductFolder() const;

    const char* GetReadMePrefix() const;

    const KMS::Version& GetVersion() const;

    bool IsEmbedded() const;

    bool IsOSIndependent() const;

    #ifdef _KMS_LINUX_

        void Init_OSDep(const KMS::DI::Array* aPackages);

        const KMS::DI::Array* GetPackages() const;

    #endif

    #ifdef _KMS_WINDOWS_

        void Init_OSDep(const char* aCertificatSHA1, uint32_t aVisualStudioVersion);

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
    std::string       mReadMePrefix;
    KMS::Version      mVersion;

    #ifdef _KMS_LINUX_
        const KMS::DI::Array* mPackages;
    #endif

    #ifdef _KMS_WINDOWS_
        std::string mCertificatSHA1;
        uint32_t    mVisualStudioVersion;
    #endif

};
