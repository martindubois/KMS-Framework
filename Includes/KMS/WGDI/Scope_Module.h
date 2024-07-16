
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WGDI/Scope_Module.h

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/CLI/Module.h>
#include <KMS/WGDI/Scope.h>

namespace KMS
{
    namespace WGDI
    {

        class Scope_Module final : public CLI::Module
        {

        public:

            Scope_Module();

            ~Scope_Module();

            void AddChannel(KMS::Scope::Channel* aChannel);

            // ===== CLI::Module ============================================
            virtual void DisplayHelp(FILE* aFile) const;
            virtual int  ExecuteCommand(CLI::CommandLine* aCmd);

        private:

            // ===== Commands ===============================================
            int Cmd                  (CLI::CommandLine* aCmd);
            int Cmd_Channel          (CLI::CommandLine* aCmd);
            int Cmd_Channel_Color    (CLI::CommandLine* aCmd);
            int Cmd_Channel_EdgeType (CLI::CommandLine* aCmd);
            int Cmd_Channel_Random   (CLI::CommandLine* aCmd);
            int Cmd_Channel_ScaleY   (CLI::CommandLine* aCmd);
            int Cmd_Channel_TrigLevel(CLI::CommandLine* aCmd);
            int Cmd_Channel_Y        (CLI::CommandLine* aCmd);
            int Cmd_Clear            (CLI::CommandLine* aCmd);
            int Cmd_Delete           (CLI::CommandLine* aCmd);
            int Cmd_ForceTrig        (CLI::CommandLine* aCmd);
            int Cmd_Frequency        (CLI::CommandLine* aCmd);
            int Cmd_Info             (CLI::CommandLine* aCmd);
            int Cmd_Mode             (CLI::CommandLine* aCmd);
            int Cmd_Persistence      (CLI::CommandLine* aCmd);
            int Cmd_PositionX        (CLI::CommandLine* aCmd);
            int Cmd_ScaleX           (CLI::CommandLine* aCmd);
            int Cmd_Select           (CLI::CommandLine* aCmd);
            int Cmd_Show             (CLI::CommandLine* aCmd);
            int Cmd_Start            (CLI::CommandLine* aCmd);
            int Cmd_Stop             (CLI::CommandLine* aCmd);

            typedef std::map<std::string, Scope*> ScopeMap;

            KMS::Scope::Channel* mChannel;
            Scope              * mScope;
            ScopeMap             mScopes;

        };

    }
}