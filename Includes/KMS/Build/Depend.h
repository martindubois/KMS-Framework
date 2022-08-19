
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Make.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Build
    {

        class Depend
        {

        public:

            Depend(const StringSet& aIncludes, const File::Folder& aFolder);

            ~Depend();

            StringSet* ParseFile(const char* aFile);

        private:

            Depend(const Depend&);

            const Depend& operator = (const Depend&);

            void AddDependency(StringSet* aInOut, const char* aFile);

            typedef std::map<std::string, StringSet*> FileMap;

            FileMap mCache;

            const File::Folder& mFolder;

            const StringSet& mIncludes;

        };

    }
}
