
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_File_UTF16.cpp

// TEST COVERAGE 2022-10-02 KMS - Martin Dubois, P. Eng.

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

        void File_UTF16::AddLine(const wchar_t* aLine)
        {
            assert(NULL != aLine);

            mLines.push_back(aLine);
        }

        void File_UTF16::Clear() { mLines.clear(); }

        const wchar_t* File_UTF16::GetLine(unsigned int aNo) const
        {
            KMS_EXCEPTION_ASSERT(mLines.size() > aNo, ARGUMENT, "Invalide line number", aNo);

            return mLines[aNo].c_str();
        }

        unsigned int File_UTF16::GetLineCount() const { return static_cast<unsigned int>(mLines.size()); }

        void File_UTF16::InsertLine(unsigned int aNo, const wchar_t* aLine)
        {
            assert(NULL != aLine);

            KMS_EXCEPTION_ASSERT(mLines.size() >= aNo, ARGUMENT, "Invalide line number", aNo);

            Internal::const_iterator lIt = mLines.begin();

            lIt += aNo;

            mLines.insert(lIt, aLine);
        }

        unsigned int File_UTF16::RemoveEmptyLines() { return RemoveLines(std::wregex(L"[ \t]*$")); }

        void File_UTF16::RemoveLines(unsigned int aNo, unsigned int aCount)
        {
            KMS_EXCEPTION_ASSERT(mLines.size() >= aNo + aCount, ARGUMENT, "Invalide line number", aNo);

            Internal::const_iterator lFirst = mLines.begin() + aNo;
            Internal::const_iterator lLast  = lFirst + aCount;

            mLines.erase(lFirst, lLast);
        }

        void File_UTF16::ReplaceLine(unsigned int aNo, const wchar_t* aLine)
        {
            assert(NULL != aLine);

            KMS_EXCEPTION_ASSERT(mLines.size() > aNo, ARGUMENT, "Invalide line number", aNo);

            mLines[aNo] = aLine;
        }

        unsigned int File_UTF16::ReplaceLines(const wchar_t* aRegEx, const wchar_t* aReplace)
        {
            assert(NULL != aRegEx);
            assert(NULL != aReplace);

            unsigned int lResult = 0;

            std::wregex lRegEx(aRegEx);

            for (std::wstring& lLine : mLines)
            {
                if (std::regex_match(lLine, lRegEx))
                {
                    lLine = aReplace;
                    lResult++;
                }
            }

            return lResult;
        }

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
                lStream << lLine << "\n";
            }
        }

        unsigned int File_UTF16::CountOccurrence(const wchar_t* aStr) const
        {
            assert(NULL != aStr);

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

        unsigned int File_UTF16::RemoveComments_CPP   () { return RemoveLines(std::wregex(L"[ \t]*//.*")); }
        unsigned int File_UTF16::RemoveComments_Script() { return RemoveLines(std::wregex(L"[ \t]*#.*" )); }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int File_UTF16::RemoveLines(const std::wregex& aRegEx)
        {
            unsigned int lResult = 0;

            Internal::iterator lIt = mLines.begin();
            while (mLines.end() != lIt)
            {
                if (std::regex_match(*lIt, aRegEx))
                {
                    // NOT TESTED
                    lIt = mLines.erase(lIt);
                    lResult++;
                }
                else
                {
                    lIt++;
                }
            }

            return lResult;
        }

    }
}
