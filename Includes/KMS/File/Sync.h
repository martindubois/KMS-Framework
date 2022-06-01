
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/Sync.h

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>

namespace KMS
{
    namespace File
    {

        class FileInfoList;

        class Sync : public Config::Configurable
        {

        public:

            static int Main(int aCount, const char ** aVector);

            Sync();

            void AddDestination(const char *aD);
            void AddFolder(const char* aF);
            void AddSource(const char* aS);

            void ClearDestinations();
            void ClearFolders();
            void ClearSources();

            void SetDestination(const char* aD);
            void SetSource     (const char* aS);

            int Run();

            // ===== Config::IConfigurable ==================================
            virtual ~Sync();
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual bool SetAttribute(const char* aA);
            virtual bool SetAttribute(const char* aA, const char* aV);

        // internal

            typedef std::list<FileInfoList *> FolderList;

        private:

            Sync(const Sync &);

            const Sync & operator = (const Sync &);

            void Run_Bidirectional();
            void Run_Unidirectional();

            void Run_Bidirectional(FileInfoList * aA, FileInfoList * aB);

            void VerifyConfig() const;

            // ===== Configurable attributes ================================
            FolderList mDestinations;
            FolderList mFolders;
            FolderList mSources;

        };

    }
}
