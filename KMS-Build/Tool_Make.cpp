
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Make.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Config.h"
#include "Phase.h"
#include "Tool.h"

#include "Tool_Make.h"

using namespace KMS;

KMS_RESULT_STATIC(RESULT_COMPILATION_FAILED);

#define FILE_EXT_A   ".a"
#define FILE_EXT_CFG ".cfg"
#define FILE_EXT_ELF ".elf"

class T_Make final : public Tool
{

public:

    T_Make(const Config& aCfg, const char* aConfiguration, const char* aProcessor);

    // ===== Tool ===========================================================
    virtual void Execute(Phase aPhase) override;

private:

    void Execute_COMPILE();

    KMS::Cfg::Configurator mConfigurator;
    KMS::Build::Make       mMake;
    KMS::Dbg::Log_Cfg      mLogCfg;

};

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CreateTool(ToolList* aTools, const Config& aCfg, const char* aConfiguration, const char* aProcessor);

namespace Tool_Make
{

    // Constants
    // //////////////////////////////////////////////////////////////////////

    const char* LIBRARY_STATIC_OUTPUT_EXTENSIONS[1] = { FILE_EXT_A };

    const char* EMBEDDED_BINARY_OUTPUT_EXTENSIONS[1] = { FILE_EXT_ELF };

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateTools(ToolList* aTools, const Config& aCfg, const DI::Array& aConfigurations, const DI::Array& aProcessors)
    {
        for (const auto& lCE : aConfigurations.mInternal)
        {
            auto lC = dynamic_cast<const DI::String*>(lCE.Get());
            assert(nullptr != lC);

            for (const auto& lPE : aProcessors.mInternal)
            {
                auto lP = dynamic_cast<const DI::String*>(lPE.Get());
                assert(nullptr != lP);

                CreateTool(aTools, aCfg, lC->Get(), lP->Get());
            }
        }
    }

}

// Public
// //////////////////////////////////////////////////////////////////////////

T_Make::T_Make(const Config& aCfg, const char* aConfiguration, const char* aProcessor)
    : Tool("Make")
    , mLogCfg(&Dbg::gLog)
{
    mConfigurator.SetSilence(Build::Make::SILENCE);

    mConfigurator.AddConfigurable(&mMake);

    mConfigurator.AddConfigurable(&mLogCfg);

    mConfigurator.ParseFile(File::Folder::CURRENT, "KMS-Build" FILE_EXT_CFG);
    mConfigurator.ParseFile(File::Folder::CURRENT, "KMS-Make"  FILE_EXT_CFG);

    mMake.mConfiguration.Set(aConfiguration);
    mMake.mProcessor    .Set(aProcessor);

    if (!aCfg.GetDoNotClean())
    {
        mMake.AddCommand("Clean");
    }

    mMake.AddCommand("Make");

    mConfigurator.Validate();
}

// ===== Tool ===============================================================

void T_Make::Execute(Phase aPhase)
{
    switch (aPhase)
    {
    case Phase::COMPILE: Execute_COMPILE(); break;
    }
}

// Private
// //////////////////////////////////////////////////////////////////////////

void T_Make::Execute_COMPILE()
{
    auto lRet = mMake.Run();
    KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_COMPILATION_FAILED, "KMS::Build::Make::Run failed", lRet);
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void CreateTool(ToolList* aTools, const Config& aCfg, const char* aConfigurtion, const char* aProcessor)
{
    assert(nullptr != aTools);

    if (!aCfg.GetDoNotCompile())
    {
        auto lTool = new T_Make(aCfg, aConfigurtion, aProcessor);

        aTools->Add(lTool);
    }
}
