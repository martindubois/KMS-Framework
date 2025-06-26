
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Folder_ToPackage.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp.h"
#include "Phase.h"

#include "Comp_Folder_ToPackage.h"

using namespace KMS;

KMS_RESULT_STATIC(RESULT_FOLDER_DOES_NOT_EXIST);

class C_Folder_ToPackage final : public Comp
{

public:

    // aSrc  This method deletes it
    C_Folder_ToPackage(File::Folder* aSrc, const char* aDst);

    // ===== Comp ===========================================================
    virtual ~C_Folder_ToPackage();
    virtual void Verify (Phase aPhase) override;
    virtual void Package(File::Folder* aTmpFolder) override;

private:

    void Verify_VERIFY();

    std::string   mDst;
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

void C_Folder_ToPackage::Verify(Phase aPhase)
{
    switch (aPhase)
    {
    case Phase::VERIFY: Verify_VERIFY(); break;
    }
}

void C_Folder_ToPackage::Package(KMS::File::Folder* aTmpFolder)
{
    assert(nullptr != mSrc);

    File::Folder lDst(*aTmpFolder, mDst.c_str());

    mSrc->Copy(lDst);
}

// Private
// //////////////////////////////////////////////////////////////////////////

void C_Folder_ToPackage::Verify_VERIFY()
{
    KMS_EXCEPTION_ASSERT(mSrc->DoesExist(), RESULT_FOLDER_DOES_NOT_EXIST, "The folder does not exist", mSrc->GetPath());
}
