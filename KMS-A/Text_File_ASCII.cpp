
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

        void File_ASCII::AddLine(const char* aLine)
        {
            assert(NULL != aLine);

            mLines.push_back(aLine);
        }

        void File_ASCII::Clear() { mLines.clear(); }

        const char* File_ASCII::GetLine(unsigned int aNo) const
        {
            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid line number", aNo);
            KMS_EXCEPTION_ASSERT(mLines.size() > aNo, TEXT_ARGUMENT_INVALID, lMsg, mLines.size());

            return mLines[aNo].c_str();
        }

        unsigned int File_ASCII::GetLineCount() const { return static_cast<unsigned int>(mLines.size()); }

        void File_ASCII::InsertLine(unsigned int aNo, const char* aLine)
        {
            assert(NULL != aLine);

            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid line number", aNo);
            KMS_EXCEPTION_ASSERT(mLines.size() >= aNo, TEXT_ARGUMENT_INVALID, lMsg, mLines.size());

            Internal::const_iterator lIt = mLines.begin();

            lIt += aNo;

            mLines.insert(lIt, aLine);
        }

        unsigned int File_ASCII::RemoveEmptyLines() { return RemoveLines(std::regex("[ \t]*$")); }

        void File_ASCII::RemoveLines(unsigned int aNo, unsigned int aCount)
        {
            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid line number", aNo);
            KMS_EXCEPTION_ASSERT(mLines.size() >= aNo + aCount, TEXT_ARGUMENT_INVALID, lMsg, mLines.size());

            Internal::const_iterator lFirst = mLines.begin() + aNo;
            Internal::const_iterator lLast = lFirst + aCount;

            mLines.erase(lFirst, lLast);
        }

        void File_ASCII::ReplaceLine(unsigned int aNo, const char* aLine)
        {
            assert(NULL != aLine);

            char lMsg[64];
            sprintf_s(lMsg, "%u is not a valid line number", aNo);
            KMS_EXCEPTION_ASSERT(mLines.size() > aNo, TEXT_ARGUMENT_INVALID, lMsg, mLines.size());

            mLines[aNo] = aLine;
        }

        unsigned int File_ASCII::ReplaceLines(const char* aRegEx, const char* aReplace)
        {
            assert(NULL != aRegEx);
            assert(NULL != aReplace);

            unsigned int lResult = 0;

            std::regex lRegEx(aRegEx);

            for (std::string& lLine : mLines)
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
            assert(NULL != aFile);

            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            std::ifstream lStream(lPath);

            char lMsg[64 + PATH_LENGTH];
            sprintf_s(lMsg, "Cannot open \"%s\" for reading", lPath);
            KMS_EXCEPTION_ASSERT(lStream.is_open(), TEXT_OPEN_FAILED, lMsg, "");

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

            char lMsg[64 + PATH_LENGTH];
            sprintf_s(lMsg, "Cannot open \"%s\" for writing", lPath);
            KMS_EXCEPTION_ASSERT(lStream.is_open(), TEXT_OPEN_FAILED, lMsg, "");

            for (std::string lLine : mLines)
            {
                lStream << lLine << "\n";
            }
        }

        unsigned int File_ASCII::CountOccurrence(const char* aStr) const
        {
            assert(NULL != aStr);

            unsigned int lResult = 0;

            for (const std::string& lLine : mLines)
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

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int File_ASCII::RemoveLines(const std::regex& aRegEx)
        {
            unsigned int lResult = 0;

            Internal::iterator lIt = mLines.begin();
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
