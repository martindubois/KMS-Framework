
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/File_ASCII.h

#pragma once

// ===== C++ ================================================================
#include <regex>

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Text
    {

        class File_ASCII
        {

        public:

            File_ASCII();

            void Read (const File::Folder& aFolder, const char* aFileName);
            void Write(const File::Folder& aFolder, const char* aFileName);

            void RemoveComments_CPP   ();
            void RemoveComments_Script();

            void RemoveEmptyLines();

            void ReplaceLines(const char* aRegEx, const char* aReplace);

        // Internal

            StringList_ASCII mLines;

        private:

            File_ASCII(const File_ASCII&);

            const File_ASCII& operator = (const File_ASCII&);

            void RemoveLines(const std::regex& aRegEx);

        };

    }
}
