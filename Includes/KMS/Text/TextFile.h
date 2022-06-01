
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/TextFile.h

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

        class TextFile
        {

        public:

            TextFile();

            void Read (const File::Folder & aFolder, const char * aFileName);
            void Write(const File::Folder & aFolder, const char * aFileName);

            void RemoveComments_CPP   ();
            void RemoveComments_Script();

            void RemoveEmptyLines();

            void ReplaceLines(const char * aRegEx, const char * aReplace);

            StringList mLines;

        private:

            TextFile(const TextFile&);

            const TextFile & operator = (const TextFile&);

            void RemoveLines(const std::regex & aRegEx);

        };

    }
}