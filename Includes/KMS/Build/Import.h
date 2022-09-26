
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Import.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Alias.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Build
    {
        
        class Import : public DI::Dictionary
        {

        public:

            static int Main(int aCount, const char** aVector);

            Import();

            ~Import();

            // aD  The dependecy format is {Product} ; {Version}
            //     (Ex: KMS-framework;0.0.14-front-end)
            void AddDependency       (const char* aD);
            void AddOSIndependentDeps(const char* aD);

            // aR  Complete repositorty path
            //     (can include environment variable)
            void AddRepository(const char* aR);

            // aDependency  The dependecy format is {Product} ; {Version}
            //              (Ex: KMS-framework;0.0.14-front-end)
            void ImportDependency(const char* aDependency, bool aOSIndependent);

            int Run();

        private:

            Import(const Import &);

            const Import & operator = (const Import &);

            File::Folder mImport;

            // ===== Configurable attributes ================================
            DI::Array mDependencies;
            DI::Array mOSIndependentDeps;
            DI::Array mRepositories;

            DI::Alias mOSDependencies;

        };

    }
}
