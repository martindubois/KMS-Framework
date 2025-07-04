
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Archive.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Build/Package.h>

// ===== Local ==============================================================
#include "Comp.h"
#include "Config.h"
#include "Error.h"
#include "Phase.h"
#include "Tool.h"

#include "Comp_Archive.h"

using namespace KMS;

class C_Archive final : public Comp
{

public:

    // aProductFolder  This method deletes it
    C_Archive(const Config& aCfg);

    // ===== Comp ===========================================================
    virtual ~C_Archive();
    virtual void Verify(Phase aPhase) override;
    virtual void Export() override;

    File::Folder mTmp_Root;

private:

    void Verify_VERIFY();

    char          mFileName[FILE_LENGTH];
    File::Folder* mProductFolder;

};

class T_Archive final : public Tool
{

public:

    T_Archive(CompList* aComps, File::Folder* aTmp_Root);

    // ===== Tool ===========================================================
    virtual void Execute(Phase aPhase) override;

private:

    void Execute_PACKAGE();

    CompList* mComps;
    
    File::Folder* mTmp_Root;

};

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static C_Archive* CreateComponent(CompList* aComps, const Config& aCfg);
static void       CreateTool     (ToolList* aTools, const Config& aCfg, CompList* aComps, File::Folder* aTmp_Root);

namespace Comp_Archive
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void GetBinaryFolder(char* aOut, unsigned int aOutSize_byte, const char* aConfiguration, const char* aProcessor)
    {
        assert(nullptr != aOut);
        assert(0 < aOutSize_byte);
        assert(nullptr != aConfiguration);
        assert(nullptr != aProcessor);

        sprintf_s(aOut SizeInfo(aOutSize_byte), "Binaries/%s_%s", aConfiguration, aProcessor);
    }

    void GetDriverFolder(char* aOut, unsigned int aOutSize_byte, const char* aConfiguration, const char* aProcessor)
    {
        assert(nullptr != aOut);
        assert(0 < aOutSize_byte);
        assert(nullptr != aConfiguration);
        assert(nullptr != aProcessor);

        sprintf_s(aOut SizeInfo(aOutSize_byte), "Drivers/%s_%s", aConfiguration, aProcessor);
    }

    void GetLibraryFolder(char* aOut, unsigned int aOutSize_byte, const char* aConfiguration, const char* aProcessor)
    {
        assert(nullptr != aOut);
        assert(0 < aOutSize_byte);
        assert(nullptr != aConfiguration);
        assert(nullptr != aProcessor);

        sprintf_s(aOut SizeInfo(aOutSize_byte), "Libraries/%s_%s", aConfiguration, aProcessor);
    }

    void CreateComponentAndTool(CompList* aComps, ToolList* aTools, const Config& aCfg)
    {
        if ((!aCfg.GetDoNotExport()) || (!aCfg.GetDoNotPackage()))
        {
            auto lComp = CreateComponent(aComps, aCfg);
            assert(nullptr != lComp);

            CreateTool(aTools, aCfg, aComps, &lComp->mTmp_Root);
        }
    }

}

// Public
// //////////////////////////////////////////////////////////////////////////

C_Archive::C_Archive(const Config& aCfg)
    : Comp("Archive")
    , mProductFolder(aCfg.GetProductFolder())
    , mTmp_Root(File::Folder::Id::TEMPORARY)
{
    Build::Package lPackage(aCfg.IsEmbedded() || aCfg.IsOSIndependent());

    lPackage.Set(aCfg.GetProduct(), aCfg.GetVersion());

    lPackage.GetFileName(mFileName, sizeof(mFileName));
}

T_Archive::T_Archive(CompList* aComps, File::Folder* aTmp_Root)
    : Tool("Archive"), mComps(aComps), mTmp_Root(aTmp_Root)
{
    assert(nullptr != aComps);
    assert(nullptr != aTmp_Root);
}

// ===== Comp ===============================================================

C_Archive::~C_Archive()
{
    assert(nullptr != mProductFolder);

    delete mProductFolder;
}

void C_Archive::Verify(Phase aPhase)
{
    switch (aPhase)
    {
    case Phase::VERIFY: Verify_VERIFY(); break;
    }
}

void C_Archive::Export()
{
    assert(nullptr != mProductFolder);

    mTmp_Root.Compress(*mProductFolder, mFileName);
}

// ===== Tool ===============================================================

void T_Archive::Execute(Phase aPhase)
{
    switch (aPhase)
    {
    case Phase::PACKAGE: Execute_PACKAGE(); break;
    }
}

// Private
// //////////////////////////////////////////////////////////////////////////

void C_Archive::Verify_VERIFY()
{
    if (mProductFolder->DoesFileExist(mFileName))
    {
        Error_File_AlreadyExist(mFileName);
    }
}

void T_Archive::Execute_PACKAGE()
{
    assert(nullptr != mComps);

    File::Folder lBinaries (*mTmp_Root, "Binaries" ); lBinaries .Create();
    File::Folder lDrivers  (*mTmp_Root, "Drivers"  ); lDrivers  .Create();
    File::Folder lLibraries(*mTmp_Root, "Libraries"); lLibraries.Create();

    mComps->Package(mTmp_Root);
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

C_Archive* CreateComponent(CompList* aComps, const Config& aCfg)
{
    assert(nullptr != aComps);
    
    auto lResult = new C_Archive(aCfg);

    aComps->Add(lResult);

    return lResult;
}

void CreateTool(ToolList* aTools, const Config& aCfg, CompList* aComps, File::Folder* aTmp_Root)
{
    assert(nullptr != aTools);

    if (!aCfg.GetDoNotPackage())
    {
        auto lTool = new T_Archive(aComps, aTmp_Root);

        aTools->Add(lTool);
    }
}
