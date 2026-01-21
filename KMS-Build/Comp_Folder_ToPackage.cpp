
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Folder_ToPackage.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp.h"
#include "Error.h"
#include "Phase.h"

#include "Comp_Folder_ToPackage.h"

using namespace KMS;

class C_Folder_ToPackage final : public Comp
{

public:

    // aSrc  This method deletes it
    C_Folder_ToPackage(File::Folder* aSrc, const char* aDst);

    // ===== Comp ===========================================================
    virtual ~C_Folder_ToPackage();
    virtual void Verify (Phase aPhase, Script::Script* aScript) override;
    virtual void Package(File::Folder* aTmpFolder, Script::Script* aScript) override;

private:

    void Verify_COMPILE(Script::Script* aScript);

    String_ASCII  mDst;
    File::Folder* mSrc;

};

namespace Comp_Folder_ToPackage
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateComponent(CompList* aComps, const Config& aCfg, const char* aFolder)
    {
        assert(nullptr != aComps);
        
        char lDst[NAME_LENGTH];
        char lSrc[NAME_LENGTH];

        if (2 != sscanf_s(aFolder, "%[^;];%[^\n\r]", lSrc SizeInfo(lSrc), lDst SizeInfo(lDst)))
        {
            KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid folder copy operation", aFolder);
        }

        auto lComp = new C_Folder_ToPackage(new File::Folder(File::Folder::CURRENT, lSrc), lDst);

        aComps->Add(lComp);
    }

    void CreateComponents(CompList* aComps, const Config& aCfg, const DI::Array& aFolders)
    {
        for (const auto& lFE : aFolders.mInternal)
        {
            auto lF = dynamic_cast<const DI::String*>(lFE.Get());
            assert(nullptr != lF);

            CreateComponent(aComps, aCfg, lF->Get());
        }
    }

}

// Public
// //////////////////////////////////////////////////////////////////////////

C_Folder_ToPackage::C_Folder_ToPackage(File::Folder* aSrc, const char* aDst)
    : Comp("Folder"), mDst(aDst), mSrc(aSrc)
{
    assert(nullptr != aSrc);
    assert(nullptr != aDst);
}

// ===== Comp ===============================================================

C_Folder_ToPackage::~C_Folder_ToPackage()
{
    assert(nullptr != mSrc);

    delete mSrc;
}

void C_Folder_ToPackage::Verify(Phase aPhase, Script::Script* aScript)
{
    switch (aPhase)
    {
    case Phase::COMPILE: Verify_COMPILE(aScript); break;
    }
}

// SCRIPT  TODO  The script does not package to a temporary folder.
void C_Folder_ToPackage::Package(KMS::File::Folder* aTmpFolder, Script::Script* aScript)
{
    assert(nullptr != mSrc);

    File::Folder lDst(*aTmpFolder, mDst.c_str());

    mSrc->Copy(lDst);
}

// Private
// //////////////////////////////////////////////////////////////////////////

// SCRIPT  Verify the needed file exist
void C_Folder_ToPackage::Verify_COMPILE(Script::Script* aScript)
{
    assert(nullptr != mSrc);

    if (!mSrc->DoesExist())
    {
        auto lPath = mSrc->GetPath();
        assert(nullptr != lPath);

        Error_Folder_DoesNotExist(lPath);

        if (nullptr != aScript)
        {
            aScript->Write_Verify_Exist(lPath);
        }
    }
}
