
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

            Depend(const StringSet_ASCII& aIncludes, const File::Folder& aFolder);

            ~Depend();

            StringSet_ASCII* ParseFile(const char* aFile);

        private:

            Depend(const Depend&);

            const Depend& operator = (const Depend&);

            void AddDependency(StringSet_ASCII* aInOut, const char* aFile);

            typedef std::map<std::string, StringSet_ASCII*> FileMap;

            FileMap mCache;

            const File::Folder& mFolder;

            const StringSet_ASCII& mIncludes;

        };

    }
}
