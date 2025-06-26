
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Export.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Comp.h"
#include "Config.h"
#include "Phase.h"
#include "Tool.h"

#include "Tool_Export.h"

using namespace KMS;

class T_Export final : public Tool
{

public:

    T_Export(CompList* aComps, const Config& aCfg);

    // ===== Tool ===========================================================
    virtual void Execute(Phase aPhase) override;

private:

    void Execute_TOOL_EXPORT();

    CompList              * mComps;
    const KMS::File::Folder mExportFolder;
    std::string             mProduct;

};

namespace Tool_Export
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateTool(ToolList* aTools, const Config& aCfg, CompList* aComps)
    {
        assert(nullptr != aTools);

        if (!aCfg.GetDoNotExport())
        {
            auto lTool = new T_Export(aComps, aCfg);

            aTools->Add(lTool);
        }
    }

}

// Public
// //////////////////////////////////////////////////////////////////////////

T_Export::T_Export(CompList* aComps, const Config& aCfg)
    : Tool("Export"), mComps(aComps), mExportFolder(aCfg.GetExportFolder()), mProduct(aCfg.GetProduct())
{
    assert(nullptr != aComps);
}

// ===== Tool ===============================================================

void T_Export::Execute(Phase aPhase)
{
    switch (aPhase)
    {
    case Phase::EXPORT: Execute_TOOL_EXPORT(); break;
    }
}

// Private
// //////////////////////////////////////////////////////////////////////////

void T_Export::Execute_TOOL_EXPORT()
{
    assert(nullptr != mComps);

    File::Folder lProductFolder(mExportFolder, mProduct.c_str());
    if (!lProductFolder.DoesExist())
    {
        lProductFolder.Create();
    }

    mComps->Export();
}
