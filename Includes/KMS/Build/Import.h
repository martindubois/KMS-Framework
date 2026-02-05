
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Import.h
// Library   KMS-B

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Folder.h>
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Build
    {

        class Package;

        /// \brief TODO Move to KMS-Import project
        class Import final : public DI::Dictionary
        {

        public:

            static const File::Folder REPOSITORY_DEFAULT;
            static const char       * SERVER_DEFAULT;

        private:

            DI::Array mDependencies;
            DI::Array mOSIndependentDeps;
            DI::Array mRepositories;
            DI::Array mServers;

        public:

            static int Main(int aCount, const char** aVector);

            Import();

            // aDependency  The dependecy format is {Product} ; {Version}
            //              (Ex: KMS-framework;0.0.14-front-end)
            void ImportDependency(const char* aDependency, bool aOSIndependent);

            int Run();

            // ===== DI::Container ==========================================
            virtual void Validate() const;

        private:

            NO_COPY(Import);

            bool ImportDependency_Folder(const Package& aPackage, const DI::Folder* aFolder, const char* aFileName);
            bool ImportDependency_Server(const Package& aPackage, const char* aServer      , const char* aFileName);

            File::Folder mImport;
            File::Folder mTmpFolder;

        };

    }
}
