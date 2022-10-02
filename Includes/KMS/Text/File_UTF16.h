
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/File_UTF16.h

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

        class File_UTF16
        {

        public:

            File_UTF16();

            void Read (const File::Folder& aFolder, const char* aFileName);
            void Write(const File::Folder& aFolder, const char* aFileName);

            void RemoveEmptyLines();

            void ReplaceLines(const wchar_t* aRegEx, const wchar_t* aReplace);

            void SwapByteOrder();

            unsigned int CountOccurrence(const wchar_t* aStr) const;

        // Internal

            StringList_UTF16 mLines;

        private:

            File_UTF16(const File_UTF16&);

            const File_UTF16& operator = (const File_UTF16&);

            void RemoveLines(const std::wregex& aRegEx);

        };

    }
}
