
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
            virtual int  ExecuteCommand(const char* aC);

        private:

            // ===== Commands ===============================================
            void Clear();
            void Channel_Color(const char* aColor);
            void Channel_EdgeType(const char* aEdgeType);
            void Channel_Random();
            void Channel_ScaleY(double aScaleY);
            void Channel_TrigLevel(double aTrigLevel);
            void Channel_Y(unsigned int aY_px);
            void Delete();
            void ForceTrig();
            void Frequency(unsigned int aFrequency_Hz);
            void Info();
            void Mode(const char* aMode);
            void Persistence(unsigned int aPersistence);
            void PositionX(unsigned int aPositionX_px);
            void ScaleX(unsigned int aScaleX_us_px);
            void Select(const char* aName);
            void Show();
            void Start();
            void Stop();

            typedef std::map<std::string, Scope*> ScopeMap;

            KMS::Scope::Channel* mChannel;
            Scope              * mScope;
            ScopeMap             mScopes;

        };

    }
}