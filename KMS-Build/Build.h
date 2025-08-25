
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Build.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Folder.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/File/Folder.h>
#include <KMS/Types.h>
#include <KMS/Version.h>

// ===== Local ==============================================================
#include "Comp.h"
#include "Tool.h"

class Config;

class Build final : public KMS::DI::Dictionary
{

public:

    static const bool  DO_NOT_CLEAN_DEFAULT;
    static const bool  DO_NOT_COMPILE_DEFAULT;
    static const bool  DO_NOT_EXPORT_DEFAULT;
    static const bool  DO_NOT_PACKAGE_DEFAULT;
    static const bool  DO_NOT_TEST_DEFAULT;
    static const char* EMBEDDED_DEFAULT;
    static const char* EXPORT_FOLDER_DEFAULT;
    static const bool  OS_INDEPENDENT_DEFAULT;
    static const char* PRODUCT_DEFAULT;
    static const char* READ_ME_PREFIX_DEFAULT;
    static const char* VERSION_FILE_DEFAULT;

    KMS::DI::Array   mBinaries;
    KMS::DI::Array   mConfigurations;
    KMS::DI::Array   mDynamicLibraries;
    KMS::DI::Boolean mDoNotClean;
    KMS::DI::Boolean mDoNotCompile;
    KMS::DI::Boolean mDoNotExport;
    KMS::DI::Boolean mDoNotPackage;
    KMS::DI::Boolean mDoNotTest;
    KMS::DI::Array   mDrivers;
    KMS::DI::Array   mEditOperations;
    KMS::DI::String  mEmbedded;
    KMS::DI::Folder  mExportFolder;
    KMS::DI::Array   mFiles;
    KMS::DI::Array   mFolders;
    KMS::DI::Array   mLibraries;
    KMS::DI::Boolean mOSIndependent;
    KMS::DI::Array   mPreBuildCmds;
    KMS::DI::Array   mProcessors;
    KMS::DI::String  mProduct;
    KMS::DI::String  mReadMePrefix;
    KMS::DI::String  mVersionFile;
    KMS::DI::Array   mTests;

    #ifdef _KMS_LINUX_
        KMS::DI::Array mPackages;
    #endif

    #ifdef _KMS_WINDOWS_
        static const char   * CERTIFICAT_SHA1_DEFAULT;
        static const uint32_t VISUAL_STUDIO_VERSION_DEFAULT;

        KMS::DI::String_Expand  mCertificatSHA1;
        KMS::DI::UInt<uint32_t> mVisualStudioVersion;
    #endif

public:

    static int Main(int aCount, const char** aVector);

    Build();

    int Run();

    // ===== DI::Container ==========================================
    virtual void Validate() const;

private:

    NO_COPY(Build);

    void Construct_OSDep();

    bool IsEmbedded() const;

    Config* CreateConfig();

    void CreateLists();
    void CreateLists_OSDep();

    CompList mComps;
    ToolList mTools;
    KMS::Version mVersion;

};
