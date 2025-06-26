
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Build_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

// ===== Local ==============================================================
#include "Comp_Installer_W.h"
#include "Config.h"
#include "Tool_VisualStudio_W.h"

#include "Build.h"

using namespace KMS;

// Constants
// /////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_CERTIFICAT_SHA1      ("CertificatSHA1 = {SHA1}");
static const KMS::Cfg::MetaData MD_VISUAL_STUDIO_VERSION("VisualStudioVersion = 2019|2022");

// Public
// //////////////////////////////////////////////////////////////////////////

const char   * Build::CERTIFICAT_SHA1_DEFAULT       = "B71CF3BCD4E228FEFD58B2FE3353EF31106C1754";
const char   * Build::EXPORT_FOLDER_DEFAULT         = "K:\\Export";
const uint32_t Build::VISUAL_STUDIO_VERSION_DEFAULT = 2022;

void Build::Construct_OSDep()
{
    mCertificatSHA1      = CERTIFICAT_SHA1_DEFAULT;
    mExportFolder        = EXPORT_FOLDER_DEFAULT;
    mVisualStudioVersion = VISUAL_STUDIO_VERSION_DEFAULT;

    Ptr_OF<DI::Object> lEntry(&mCertificatSHA1, false);
    AddEntry("CertificatSHA1", lEntry, &MD_CERTIFICAT_SHA1);

    lEntry.Set(&mVisualStudioVersion, false);
    AddEntry("VisualStudioVersion", lEntry, &MD_VISUAL_STUDIO_VERSION);
}

// Private
// //////////////////////////////////////////////////////////////////////////

void Build::CreateLists_OSDep()
{
    Config lCfg(mCertificatSHA1, mDoNotClean, mDoNotCompile, mDoNotExport, mDoNotPackage, mDoNotTest, IsEmbedded(), mExportFolder, mOSIndependent, mProduct, mVersion, mVisualStudioVersion);

    Comp_Installer::CreateComponentsAndTools(&mComps, &mTools, lCfg, mProcessors);

    if ((!IsEmbedded()) && (!mDoNotCompile))
    {
        Tool_VisualStudio::CreateTools(&mTools, lCfg, mConfigurations, mProcessors);
    }
}
