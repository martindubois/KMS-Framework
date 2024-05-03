
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_File_UTF16.cpp

// TEST COVERAGE 2023-07-27 Martin Dubois

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
            assert(nullptr != aLine);

            mLines.push_back(aLine);
        }

        const wchar_t* File_UTF16::GetLine(unsigned int aNo) const
        {
            if (mLines.size() <= aNo)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid line number", aNo);
                KMS_EXCEPTION(RESULT_INVALID_LINE_NO, lMsg, mLines.size());
            }

            return mLines[aNo].c_str();
        }

        void File_UTF16::InsertLine(unsigned int aNo, const wchar_t* aLine)
        {
            assert(nullptr != aLine);

            if (mLines.size() < aNo)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid line number", aNo);
                KMS_EXCEPTION(RESULT_INVALID_LINE_NO, lMsg, mLines.size());
            }

            auto lIt = mLines.begin();

            lIt += aNo;

            mLines.insert(lIt, aLine);
        }

        void File_UTF16::ReplaceLine(unsigned int aNo, const wchar_t* aLine)
        {
            assert(nullptr != aLine);

            if (mLines.size() <= aNo)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid line number", aNo);
                KMS_EXCEPTION(RESULT_INVALID_LINE_NO, lMsg, mLines.size());
            }

            mLines[aNo] = aLine;
        }

        unsigned int File_UTF16::ReplaceLines(const wchar_t* aRegEx, const wchar_t* aReplace)
        {
            assert(nullptr != aRegEx);
            assert(nullptr != aReplace);

            unsigned int lResult = 0;

            std::wregex lRegEx(aRegEx);

            for (auto& lLine : mLines)
            {
                if (std::regex_match(lLine, lRegEx))
                {
                    lLine = aReplace;
                    lResult++;
                }
            }

            return lResult;
        }

        void File_UTF16::Write(const KMS::File::Folder& aFolder, const char* aFile, const wchar_t* aEOL)
        {
            assert(nullptr != aEOL);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::wofstream lStream(lPath, std::ios::binary);
            if (!lStream.is_open())
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open \"%s\" for writing", lPath);
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, "");
            }

            lStream.imbue(std::locale(lStream.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::generate_header>));

            for (const auto& lLine : mLines)
            {
                lStream << lLine << aEOL;
            }
        }

        void File_UTF16::Write_ASCII(const KMS::File::Folder& aFolder, const char* aFile, const char* aEOL)
        {
            assert(nullptr != aEOL);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            FILE* lFile;

            auto lErr = fopen_s(&lFile, lPath, "wb");
            if (0 != lErr)
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open \"%s\" for writing", lPath);
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, lErr);
            }

            for (const auto& lLine : mLines)
            {
                fprintf(lFile, "%S%s", lLine.c_str(), aEOL);
            }

            auto lRet = fclose(lFile);
            assert(0 == lRet);
        }

        unsigned int File_UTF16::CountOccurrence(const wchar_t* aStr) const
        {
            assert(nullptr != aStr);

            unsigned int lResult = 0;

            for (auto& lLine : mLines)
            {
                if (lLine.npos != lLine.find(aStr))
                {
                    lResult++;
                }
            }

            return lResult;
        }

        // ===== File =======================================================

        void File_UTF16::Clear() { mLines.clear(); }

        unsigned int File_UTF16::GetLineCount() const { return static_cast<unsigned int>(mLines.size()); }

        unsigned int File_UTF16::GetUserLineNo(unsigned int aNo) const
        {
            if (mLines.size() <= aNo)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid line number", aNo);
                KMS_EXCEPTION(RESULT_INVALID_LINE_NO, lMsg, mLines.size());
            }

            return mLines[aNo].GetUserLineNo();
        }

        unsigned int File_UTF16::RemoveEmptyLines() { return RemoveLines(std::wregex(L"[ \t]*$")); }

        void File_UTF16::RemoveLines(unsigned int aNo, unsigned int aCount)
        {
            if (mLines.size() < aNo + aCount)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid line number", aNo);
                KMS_EXCEPTION(RESULT_INVALID_LINE_NO, lMsg, mLines.size());
            }

            auto lFirst = mLines.begin() + aNo;
            auto lLast  = lFirst + aCount;

            mLines.erase(lFirst, lLast);
        }

        void File_UTF16::Read(const KMS::File::Folder& aFolder, const char* aFile, unsigned int aFlags)
        {
            assert(nullptr != aFile);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::wifstream lStream(lPath, std::ios::binary);
            if (!lStream.is_open())
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open \"%s\" for reading", lPath);
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, "");
            }

            lStream.imbue(std::locale(lStream.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));

            std::wstring lLine;
            unsigned int lLineNo = 0;

            while (getline(lStream, lLine))
            {
                if ((!lLine.empty()) && (L'\r' == lLine.back()) && (0 == (FLAG_DO_NOT_REMOVE_CR & aFlags)))
                {
                    // NOT TESTED
                    lLine.pop_back();
                }

                mLines.push_back(Line(lLine, lLineNo));

                lLineNo++;
            }
        }

        unsigned int File_UTF16::RemoveComments_CPP   () { return RemoveLines(std::wregex(L"[ \t]*//.*")); }
        unsigned int File_UTF16::RemoveComments_Script() { return RemoveLines(std::wregex(L"[ \t]*#.*" )); }

        // Internal
        // //////////////////////////////////////////////////////////////////

        File_UTF16::Line::Line(const wchar_t* aIn, unsigned int aUserLineNo)
            : std::wstring(aIn), mUserLineNo(aUserLineNo)
        {}

        File_UTF16::Line::Line(const std::wstring& aIn, unsigned int aUserLineNo)
            : std::wstring(aIn), mUserLineNo(aUserLineNo)
        {}

        unsigned int File_UTF16::Line::GetUserLineNo() const { return mUserLineNo; }

        const File_UTF16::Line& File_UTF16::GetLine2(unsigned int aNo) const
        {
            if (mLines.size() <= aNo)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid line number", aNo);
                KMS_EXCEPTION(RESULT_INVALID_LINE_NO, lMsg, mLines.size());
            }

            return mLines[aNo];
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int File_UTF16::RemoveLines(const std::wregex& aRegEx)
        {
            unsigned int lResult = 0;

            auto lIt = mLines.begin();
            while (mLines.end() != lIt)
            {
                if (std::regex_match(*lIt, aRegEx))
                {
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
