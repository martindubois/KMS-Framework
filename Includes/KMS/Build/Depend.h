
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2022-2026 KMS
/// \file      Includes/KMS/Build/Make.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Build
    {

        /// \brief TODO Remove use of DI namespace
        class Depend final
        {

        public:

            Depend(const DI::Array& aIncludes, const File::Folder& aFolder);

            ~Depend();

            StringSet_ASCII* ParseFile(const char* aFile);

        // Internal

            void Display(std::ostream& aOut) const;

        private:

            NO_COPY(Depend);

            void AddDependency(StringSet_ASCII* aInOut, const char* aFile);

            void LocateDependency(StringSet_ASCII* aInOut, const char* aFile);

            bool LocateLocalDependency(StringSet_ASCII* aInOut, const char* aFile, const char* aFrom);

            typedef std::map<String_ASCII, StringSet_ASCII*> FileMap;

            FileMap mCache;

            unsigned int mFileCount;

            const File::Folder& mFolder;

            const DI::Array& mIncludes;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Build::Depend& aTM);
