
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_File_UTF16.cpp

// TEST COVERAGE 2022-10-24 KMS - Martin Dubois, P. Eng.

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
            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid line number", aNo);
            KMS_EXCEPTION_ASSERT(mLines.size() > aNo, TEXT_ARGUMENT_INVALID, lMsg, mLines.size());

            return mLines[aNo].c_str();
        }

        unsigned int File_UTF16::GetLineCount() const { return static_cast<unsigned int>(mLines.size()); }

        void File_UTF16::InsertLine(unsigned int aNo, const wchar_t* aLine)
        {
            assert(NULL != aLine);

            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid line number", aNo);
            KMS_EXCEPTION_ASSERT(mLines.size() >= aNo, TEXT_ARGUMENT_INVALID, lMsg, mLines.size());

            Internal::const_iterator lIt = mLines.begin();

            lIt += aNo;

            mLines.insert(lIt, aLine);
        }

        unsigned int File_UTF16::RemoveEmptyLines() { return RemoveLines(std::wregex(L"[ \t]*$")); }

        void File_UTF16::RemoveLines(unsigned int aNo, unsigned int aCount)
        {
            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid line number", aNo);
            KMS_EXCEPTION_ASSERT(mLines.size() >= aNo + aCount, TEXT_ARGUMENT_INVALID, lMsg, mLines.size());

            Internal::const_iterator lFirst = mLines.begin() + aNo;
            Internal::const_iterator lLast  = lFirst + aCount;

            mLines.erase(lFirst, lLast);
        }

        void File_UTF16::ReplaceLine(unsigned int aNo, const wchar_t* aLine)
        {
            assert(NULL != aLine);

            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid line number", aNo);
            KMS_EXCEPTION_ASSERT(mLines.size() > aNo, TEXT_ARGUMENT_INVALID, lMsg, mLines.size());

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

            char lMsg[64 + PATH_LENGTH];
            sprintf_s(lMsg, "Cannot open \"%s\" for reading", lPath);
            KMS_EXCEPTION_ASSERT(lStream.is_open(), TEXT_OPEN_FAILED, lMsg, "");

            lStream.imbue(std::locale(lStream.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));

            std::wstring lLine;

            while (getline(lStream, lLine))
            {
                mLines.push_back(lLine);
            }
        }

        void File_UTF16::Write(const File::Folder& aFolder, const char* aFile, const wchar_t* aEOL)
        {
            assert(NULL != aEOL);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::wofstream lStream(lPath, std::ios::binary);

            char lMsg[64 + PATH_LENGTH];
            sprintf_s(lMsg, "Cannot open \"%s\" for writing", lPath);
            KMS_EXCEPTION_ASSERT(lStream.is_open(), TEXT_OPEN_FAILED, lMsg, "");

            lStream.imbue(std::locale(lStream.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::generate_header>));

            for (const std::wstring& lLine : mLines)
            {
                lStream << lLine << aEOL;
            }
        }

        void File_UTF16::Write_ASCII(const File::Folder& aFolder, const char* aFile, const char* aEOL)
        {
            assert(NULL != aEOL);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            FILE* lFile;

            errno_t lErr = fopen_s(&lFile, lPath, "wb");

            char lMsg[64 + PATH_LENGTH];
            sprintf_s(lMsg, "Cannot open \"%s\" for writing", lPath);
            KMS_EXCEPTION_ASSERT(0 == lErr, TEXT_OPEN_FAILED, lMsg, lErr);

            for (const std::wstring& lLine : mLines)
            {
                fprintf(lFile, "%S%s", lLine.c_str(), aEOL);
            }

            int lRet = fclose(lFile);
            assert(0 == lRet);
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
