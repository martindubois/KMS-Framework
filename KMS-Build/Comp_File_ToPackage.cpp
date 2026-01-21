
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_File_ToPackage.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp.h"
#include "Config.h"
#include "Error.h"
#include "Phase.h"

#include "Comp_File_ToPackage.h"

using namespace KMS;

class C_File_ToPackage final : public Comp
{

public:

    C_File_ToPackage(File::Folder* aSrc, const char* aDst, const char* aFileName, Phase aVerify);

    // ===== Comp ===========================================================
    virtual ~C_File_ToPackage();
    virtual void Verify (Phase aPhase, Script::Script* aScript) override;
    virtual void Package(KMS::File::Folder* aTmpFolder, Script::Script* aScript) override;

private:

    String_ASCII mDst;
    String_ASCII mFileName;

    File::Folder* mSrc;

    Phase mVerify;

};

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CreateComponent(CompList* aComps, const Config& aCfg, const char* aPath);

namespace Comp_File_ToPackage
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateComponent(CompList* aComps, const Config& aCfg, File::Folder* aSrc, const char* aDst, const char* aFileName, Phase aVerify)
    {
        assert(nullptr != aComps);
        
        if (!aCfg.GetDoNotPackage())
        {
            auto lComp = new C_File_ToPackage(aSrc, aDst, aFileName, aVerify);

            aComps->Add(lComp);
        }
    }

    void CreateComponents(CompList* aComps, const Config& aCfg, const DI::Array& aFiles)
    {
        for (const auto& lFE : aFiles.mInternal)
        {
            auto lF = dynamic_cast<const DI::String*>(lFE.Get());
            assert(nullptr != lF);

            CreateComponent(aComps, aCfg, lF->Get());
        }
    }

}

// Public
// //////////////////////////////////////////////////////////////////////////

C_File_ToPackage::C_File_ToPackage(File::Folder* aSrc, const char* aDst, const char* aFileName, Phase aVerify)
    : Comp("File"), mDst(aDst), mFileName(aFileName), mSrc(aSrc), mVerify(aVerify)
{
    assert(nullptr != aSrc);
    assert(nullptr != aDst);
    assert(nullptr != aFileName);
}

// ===== Comp ===============================================================

C_File_ToPackage::~C_File_ToPackage()
{
    assert(nullptr != mSrc);

    delete mSrc;
}

// SCRIPT  Verify the file to package exist
void C_File_ToPackage::Verify(Phase aPhase, Script::Script* aScript)
{
    assert(nullptr != mSrc);

    if (mVerify == aPhase)
    {
        auto lFileName = mFileName.c_str();
        assert(nullptr != lFileName);

        if (!mSrc->DoesFileExist(lFileName))
        {
            Error_File_DoesNotExist(lFileName);
        }

        if (nullptr != aScript)
        {
            char lPath[PATH_LENGTH];

            mSrc->GetPath(lFileName, lPath, sizeof(lPath));

            aScript->Write_Verify_Exist(lPath);
        }
    }
}

// SCRIPT  TODO  The script does not package to a temporary folder
void C_File_ToPackage::Package(KMS::File::Folder* aTmpFolder, Script::Script* aScript)
{
    assert(nullptr != aTmpFolder);

    assert(nullptr != mSrc);

    auto lFileName = mFileName.c_str();
    assert(nullptr != lFileName);

    if (mDst.empty())
    {
        mSrc->Copy(*aTmpFolder, lFileName);
    }
    else
    {
        File::Folder lDst(*aTmpFolder, mDst.c_str());

        if (!lDst.DoesExist())
        {
            lDst.Create();
        }

        mSrc->Copy(lDst, lFileName);
    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void CreateComponent(CompList* aComps, const Config& aCfg, const char* aPath)
{
    assert(nullptr != aPath);

    char lPath[PATH_LENGTH];

    strcpy_s(lPath, aPath);

    const char  * lFileName;
    File::Folder* lSrc;

    auto lPtr = strrchr(lPath, '/');
    if (nullptr == lPtr)
    {
        lFileName = lPath;
        lSrc = new File::Folder(File::Folder::CURRENT);
    }
    else
    {
        *lPtr = '\0';

        lFileName = lPtr + 1;
        lSrc = new File::Folder(File::Folder::CURRENT, lPath);
    }

    Comp_File_ToPackage::CreateComponent(aComps, aCfg, lSrc, "", lFileName, Phase::VERIFY);
}
