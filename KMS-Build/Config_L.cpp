
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Config_L.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Config.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

Config::Config(bool aDoNotClean, bool aDoNotCompile, bool aDoNotExport, bool aDoNotPackage, bool aDoNotTest,
    bool aEmbedded, const File::Folder aExportFolder, bool aOSIndependent, const char* aProduct, const Version& aVersion)
    : mDoNotClean  (aDoNotClean)
    , mDoNotCompile(aDoNotCompile)
    , mDoNotExport (aDoNotExport)
    , mDoNotPackage(aDoNotPackage)
    , mDoNotTest   (aDoNotTest)
    , mEmbedded(aEmbedded)
    , mExportFolder(aExportFolder)
    , mOSIndependent(aOSIndependent)
    , mProduct(aProduct)
    , mVersion(aVersion)
{
    assert(nullptr != aProduct);
}

