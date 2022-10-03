
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_File_ASCII.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <fstream>

// ===== Includes ===========================================================
#include <KMS/Text/File_ASCII.h>

namespace KMS
{
    namespace Text
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        File_ASCII::File_ASCII() {}

        void File_ASCII::Read(const File::Folder& aFolder, const char* aFile)
        {
            assert(NULL != aFile);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::ifstream lStream(lPath);
            KMS_EXCEPTION_ASSERT(lStream.is_open(), FILE_OPEN, "Cannot open text file for reading", lPath);

            std::string lLine;

            while (getline(lStream, lLine))
            {
                mLines.push_back(lLine);
            }
        }

        void File_ASCII::Write(const File::Folder& aFolder, const char* aFile)
        {
            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::ofstream lStream(lPath);
            KMS_EXCEPTION_ASSERT(lStream.is_open(), FILE_OPEN, "Cannot open text file for writing", lPath);

            for (std::string lLine : mLines)
            {
                lStream << lLine << "\n";
            }
        }

        void File_ASCII::RemoveComments_CPP   () { RemoveLines(std::regex("[ \t]*//.*")); }
        void File_ASCII::RemoveComments_Script() { RemoveLines(std::regex("[ \t]*#.*")); }

        void File_ASCII::RemoveEmptyLines() { RemoveLines(std::regex("[ \t]*$")); }

        void File_ASCII::ReplaceLines(const char* aRegEx, const char* aReplace)
        {
            assert(NULL != aRegEx);
            assert(NULL != aReplace);

            std::regex lRegEx(aRegEx);

            StringList_ASCII::iterator lIt = mLines.begin();
            while (lIt != mLines.end())
            {
                if (std::regex_match(*lIt, lRegEx))
                {
                    (*lIt) = aReplace;
                }

                lIt++;
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void File_ASCII::RemoveLines(const std::regex& aRegEx)
        {
            StringList_ASCII::iterator lIt = mLines.begin();
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
