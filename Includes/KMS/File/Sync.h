
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/Sync.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <map>

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

            ~Sync();

            void AddDestination(const char *aD);
            void AddSource(const char* aS);

            void AddFolder(const char* aG, const char* aF);

            void ClearDestinations();
            void ClearSources();

            void ClearFolders(const char* aG);

            void SetDestination(const char* aD);
            void SetSource     (const char* aS);

            int Run();

            // ===== Config::Configurable ===================================
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual bool AddAttribute_Indexed(const char* aA, const char* aI, const char* aV);
            virtual bool SetAttribute(const char* aA, const char* aV);
            virtual bool SetAttribute_Indexed(const char* aA, const char* aI, const char* aV);

        // internal

            typedef std::list<FileInfoList*> FolderList;

        private:

            typedef std::map<std::string, FolderList*> GroupMap;

            Sync(const Sync &);

            const Sync & operator = (const Sync &);

            void Run_Bidirectional();
            void Run_Unidirectional();

            void Run_Bidirectional(FileInfoList * aA, FileInfoList * aB);

            void VerifyConfig() const;

            // ===== Configurable attributes ================================
            FolderList mDestinations;
            GroupMap   mGroups;
            FolderList mSources;

        };

    }
}
