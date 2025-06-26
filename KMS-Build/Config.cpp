
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Config.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Config.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

Config::Config(const char* aCertificatSHA1, bool aDoNotClean, bool aDoNotCompile, bool aDoNotExport, bool aDoNotPackage, bool aDoNotTest,
    bool aEmbedded, const File::Folder aExportFolder, bool aOSIndependent, const char* aProduct, const Version& aVersion, uint32_t aVisualStudioVersion)
    : mCertificatSHA1(aCertificatSHA1)
    , mDoNotClean  (aDoNotClean)
    , mDoNotCompile(aDoNotCompile)
    , mDoNotExport (aDoNotExport)
    , mDoNotPackage(aDoNotPackage)
    , mDoNotTest   (aDoNotTest)
    , mEmbedded(aEmbedded)
    , mExportFolder(aExportFolder)
    , mOSIndependent(aOSIndependent)
    , mProduct(aProduct)
    , mVersion(aVersion)
    , mVisualStudioVersion(aVisualStudioVersion)
{
    assert(nullptr != aCertificatSHA1);
    assert(nullptr != aProduct);
}

const char* Config::GetCertificatSHA1() const { return mCertificatSHA1.c_str(); }

bool Config::GetDoNotClean  () const { return mDoNotClean  ; }
bool Config::GetDoNotCompile() const { return mDoNotCompile; }
bool Config::GetDoNotExport () const { return mDoNotExport ; }
bool Config::GetDoNotPackage() const { return mDoNotPackage; }
bool Config::GetDoNotTest   () const { return mDoNotTest   ; }

const File::Folder& Config::GetExportFolder() const { return mExportFolder; }

const char* Config::GetProduct() const { return mProduct.c_str(); }

const Version& Config::GetVersion() const { return mVersion; }

File::Folder* Config::GetProductFolder() const
{
    return new File::Folder(mExportFolder, GetProduct());
}

uint32_t Config::GetVisualStudioVersion() const { return mVisualStudioVersion; }

bool Config::IsEmbedded() const { return mEmbedded; }

bool Config::IsOSIndependent() const { return mOSIndependent; }
