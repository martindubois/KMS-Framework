
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_File_ToExport.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Comp.h"
#include "Config.h"
#include "Phase.h"

#include "Comp_File_ToExport.h"

using namespace KMS;

KMS_RESULT_STATIC(RESULT_FILE_ALREADY_EXIST);

class C_File_ToExport final : public Comp
{

public:

    // aFolder  The method deletes it
    C_File_ToExport(const Config& aCfg, File::Folder* aFolder, const char* aFileName);

    // ===== Comp ===========================================================
    virtual ~C_File_ToExport();
    virtual void Verify(Phase aPhase) override;
    virtual void Export() override;

private:

    virtual void Verify_VERIFY();

    std::string   mFileName;
    File::Folder* mFolder;
    File::Folder* mProductFolder;

};

namespace Comp_File_ToExport
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateComponent(CompList* aComps, const Config& aCfg, File::Folder* aFolder, const char* aFileName)
    {
        assert(nullptr != aComps);

        if (!aCfg.GetDoNotExport())
        {
            auto lComp = new C_File_ToExport(aCfg, aFolder, aFileName);

            aComps->Add(lComp);
        }
    }

}

// Public
// //////////////////////////////////////////////////////////////////////////

C_File_ToExport::C_File_ToExport(const Config& aCfg, File::Folder* aFolder, const char* aFileName)
    : Comp("File_ToExport"), mFileName(aFileName), mFolder(aFolder), mProductFolder(aCfg.GetProductFolder())
{
    assert(nullptr != aFolder);
    assert(nullptr != aFileName);
}

// ===== Comp ===============================================================

C_File_ToExport::~C_File_ToExport()
{
    assert(nullptr != mFolder);

    delete mFolder;
}

void C_File_ToExport::Verify(Phase aPhase)
{
    switch (aPhase)
    {
    case Phase::VERIFY: Verify_VERIFY(); break;
    }
}

void C_File_ToExport::Export()
{
    assert(nullptr != mFolder);
    assert(nullptr != mProductFolder);

    mFolder->Copy(*mProductFolder, mFileName.c_str());
}

// Private
// //////////////////////////////////////////////////////////////////////////

void C_File_ToExport::Verify_VERIFY()
{
    KMS_EXCEPTION_ASSERT(!mProductFolder->DoesFileExist(mFileName.c_str()), RESULT_FILE_ALREADY_EXIST, "The file to export already exist", mFileName.c_str());
}
