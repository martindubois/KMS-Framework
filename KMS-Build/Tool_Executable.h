
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Executable.h

// aVerify   Verify the executable exist
// aExecute  Execute the executable

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Proc/Process.h>

// ===== Local ==============================================================
#include "Tool.h"

class Config;

namespace Tool_Executable
{

    class Tool final : public ::Tool
    {

    public:
    
        // aFolder  This method deletes it
        Tool(KMS::File::Folder* aFolder, const char* aExecutable, uint32_t aTimeout_ms, Phase aVerify, Phase aExecute);

        void AddArgument(const char* aArg);
    
        // ===== Tool ===========================================================
        virtual ~Tool();
        virtual void Execute(Phase aPhase) override;
    
    private:

        Phase              mExecute;
        std::string        mExecutable;
        KMS::File::Folder* mFolder;
        KMS::Proc::Process mProcess;
        uint32_t           mTimeout_ms;
        Phase              mVerify;

    };

    // aFolder  This method deletes it
    Tool* CreateTool(ToolList* aTools, const Config& aCfg, KMS::File::Folder* aFolder, const char* aExecutable, uint32_t aTimeout_ms, Phase aVerify, Phase aExecute);

};
