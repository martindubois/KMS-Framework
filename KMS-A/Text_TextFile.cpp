
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_TextFile.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <fstream>

// ===== Includes ===========================================================
#include <KMS/Text/TextFile.h>

namespace KMS
{
    namespace Text
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        TextFile::TextFile() {}

        void TextFile::Read(const File::Folder& aFolder, const char* aFile)
        {
            assert(NULL != aFile);

            char lPath[1024];
            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::ifstream lStream(lPath);
            if (!lStream.is_open())
            {
                KMS_EXCEPTION_WITH_INFO(FILE_OPEN, "Cannot open text file", lPath);
            }

            std::string lLine;

            while (getline(lStream, lLine))
            {
                mLines.push_back(lLine);
            }
        }

        void TextFile::RemoveComments_CPP   () { RemoveLines(std::regex("[ \t]*//.*")); }
        void TextFile::RemoveComments_Script() { RemoveLines(std::regex("[ \t]*#.*")); }

        void TextFile::RemoveEmptyLines() { RemoveLines(std::regex("[ \t]*$")); }

        // Private
        // //////////////////////////////////////////////////////////////////

        void TextFile::RemoveLines(const std::regex& aRegEx)
        {
            StringList::iterator lIt = mLines.begin();
            while (lIt != mLines.end())
            {
                if (std::regex_match(*lIt, aRegEx))
                {
                    lIt = mLines.erase(lIt);
                }
                else
                {
                    lIt++;
                }
            }
        }

    }
}