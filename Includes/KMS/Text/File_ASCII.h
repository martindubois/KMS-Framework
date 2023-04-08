
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

            void AddLine(const char* aLine);

            void Clear();

            const char* GetLine(unsigned int aNo) const;

            unsigned int GetLineCount() const;

            void InsertLine(unsigned int aNo, const char* aLine);

            unsigned int RemoveEmptyLines();

            void RemoveLines(unsigned int aNo, unsigned int aCount);

            void ReplaceLine(unsigned int aNo, const char* aLine);

            unsigned int ReplaceLines(const char* aRegEx, const char* aReplace);

            void Read (const File::Folder& aFolder, const char* aFileName);
            void Write(const File::Folder& aFolder, const char* aFileName);

            unsigned int CountOccurrence(const char* aStr) const;

            unsigned int RemoveComments_CPP   ();
            unsigned int RemoveComments_Script();

        // Internal

            typedef std::vector<std::string> Internal;

            Internal mLines;

        private:

            NO_COPY(File_ASCII);

            unsigned int RemoveLines(const std::regex& aRegEx);

        };

    }
}
