
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_File_UTF16.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <codecvt>
#include <fstream>

// ===== Includes ===========================================================
#include <KMS/Text/File_UTF16.h>

namespace KMS
{
    namespace Text
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        File_UTF16::File_UTF16() {}

        void File_UTF16::Read(const File::Folder& aFolder, const char* aFile)
        {
            assert(NULL != aFile);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::wifstream lStream(lPath, std::ios::binary);
            KMS_EXCEPTION_ASSERT(lStream.is_open(), FILE_OPEN, "Cannot open text file for reading", lPath);

            lStream.imbue(std::locale(lStream.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));

            std::wstring lLine;

            while (getline(lStream, lLine))
            {
                mLines.push_back(lLine);
            }
        }

        void File_UTF16::Write(const File::Folder& aFolder, const char* aFile)
        {
            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::wofstream lStream(lPath, std::ios::binary);
            KMS_EXCEPTION_ASSERT(lStream.is_open(), FILE_OPEN, "Cannot open text file for writing", lPath);

            lStream.imbue(std::locale(lStream.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::generate_header>));

            for (std::wstring lLine : mLines)
            {
                lStream << lLine << std::endl;
            }
        }

        void File_UTF16::RemoveEmptyLines() { RemoveLines(std::wregex(L"[ \t]*$")); }

        void File_UTF16::ReplaceLines(const wchar_t* aRegEx, const wchar_t* aReplace)
        {
            assert(NULL != aRegEx);
            assert(NULL != aReplace);

            std::wregex lRegEx(aRegEx);

            StringList_UTF16::iterator lIt = mLines.begin();
            while (lIt != mLines.end())
            {
                if (std::regex_match(*lIt, lRegEx))
                {
                    (*lIt) = aReplace;
                }

                lIt++;
            }
        }

        unsigned int File_UTF16::CountOccurrence(const wchar_t* aStr) const
        {
            unsigned int lResult = 0;

            for (const std::wstring& lLine : mLines)
            {
                if (lLine.npos != lLine.find(aStr))
                {
                    lResult++;
                }
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void File_UTF16::RemoveLines(const std::wregex& aRegEx)
        {
            StringList_UTF16::iterator lIt = mLines.begin();
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
