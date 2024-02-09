
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Text_File_ASCII.cpp

// TEST COVERAGE 2023-05-21 Martin Dubois

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

        void File_ASCII::AddLine(const char* aLine)
        {
            assert(nullptr != aLine);

            mLines.push_back(aLine);
        }

        void File_ASCII::Clear() { mLines.clear(); }

        const char* File_ASCII::GetLine(unsigned int aNo) const
        {
            if (mLines.size() <= aNo)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid line number", aNo);
                KMS_EXCEPTION(RESULT_INVALID_LINE_NO, lMsg, mLines.size());
            }

            return mLines[aNo].c_str();
        }

        unsigned int File_ASCII::GetLineCount() const { return static_cast<unsigned int>(mLines.size()); }

        unsigned int File_ASCII::GetUserLineNo(unsigned int aNo) const
        {
            if (mLines.size() <= aNo)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is not a valid line number", aNo);
                KMS_EXCEPTION(RESULT_INVALID_LINE_NO, lMsg, mLines.size());
            }

            return mLines[aNo].GetUserLineNo();
        }

        void File_ASCII::InsertLine(unsigned int aNo, const char* aLine)
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

        unsigned int File_ASCII::RemoveEmptyLines() { return RemoveLines(std::regex("[ \t]*$")); }

        void File_ASCII::RemoveLines(unsigned int aNo, unsigned int aCount)
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

        void File_ASCII::ReplaceLine(unsigned int aNo, const char* aLine)
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

        unsigned int File_ASCII::ReplaceLines(const char* aRegEx, const char* aReplace)
        {
            assert(nullptr != aRegEx);
            assert(nullptr != aReplace);

            unsigned int lResult = 0;

            std::regex lRegEx(aRegEx);

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

        void File_ASCII::Read(const File::Folder& aFolder, const char* aFile)
        {
            assert(nullptr != aFile);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::ifstream lStream(lPath);
            if (!lStream.is_open())
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open \"%s\" for reading", lPath);
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, "");
            }

            std::string lLine;
            unsigned int lLineNo = 0;

            while (getline(lStream, lLine))
            {
                if ((!lLine.empty()) && ('\r' == lLine.back()))
                {
                    // NOT TESTED
                    lLine.pop_back();
                }

                mLines.push_back(Line(lLine, lLineNo));

                lLineNo++;
            }
        }

        void File_ASCII::Write(const File::Folder& aFolder, const char* aFile, const char* aEOL)
        {
            assert(nullptr != aEOL);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::ofstream lStream(lPath);
            if (!lStream.is_open())
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open \"%s\" for writing", lPath);
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, "");
            }

            for (const auto& lLine : mLines)
            {
                lStream << lLine << aEOL;
            }
        }

        unsigned int File_ASCII::CountOccurrence(const char* aStr) const
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

        unsigned int File_ASCII::RemoveComments_CPP   () { return RemoveLines(std::regex("[ \t]*//.*")); }
        unsigned int File_ASCII::RemoveComments_Script() { return RemoveLines(std::regex("[ \t]*#.*" )); }

        // Internal
        // //////////////////////////////////////////////////////////////////

        File_ASCII::Line::Line(const char* aIn, unsigned int aUserLineNo)
            : std::string(aIn), mUserLineNo(aUserLineNo)
        {}

        File_ASCII::Line::Line(const std::string& aIn, unsigned int aUserLineNo)
            : std::string(aIn), mUserLineNo(aUserLineNo)
        {}

        unsigned int File_ASCII::Line::GetUserLineNo() const { return mUserLineNo; }

        const File_ASCII::Line& File_ASCII::GetLine2(unsigned int aNo) const
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

        unsigned int File_ASCII::RemoveLines(const std::regex& aRegEx)
        {
            unsigned int lResult = 0;

            auto lIt = mLines.begin();
            while (lIt != mLines.end())
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
