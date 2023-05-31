
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/File_UTF16.h

#pragma once

// ===== C++ ================================================================
#include <regex>
#include <vector>

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Text
    {

        class File_UTF16
        {

        public:

            File_UTF16();

            void AddLine(const wchar_t* aLine);

            void Clear();

            const wchar_t* GetLine(unsigned int aNo) const;

            unsigned int GetLineCount() const;

            void InsertLine(unsigned int aNo, const wchar_t* aLine);

            unsigned int RemoveEmptyLines();

            void RemoveLines(unsigned int aNo, unsigned int aCount);

            void ReplaceLine(unsigned int aNo, const wchar_t* aLine);

            unsigned int ReplaceLines(const wchar_t* aRegEx, const wchar_t* aReplace);

            void Read (const File::Folder& aFolder, const char* aFileName);
            void Write(const File::Folder& aFolder, const char* aFileName, const wchar_t* aEOL = L"\n");

            void Write_ASCII(const File::Folder& aFolder, const char* aFileName, const char* aEOL = "\n");

            unsigned int CountOccurrence(const wchar_t* aStr) const;

            unsigned int RemoveComments_CPP   ();
            unsigned int RemoveComments_Script();

        // Internal

            class Line : public std::wstring
            {

            public:

                Line(const wchar_t     * aIn, unsigned int aUserLineNo = 0xffffffff);
                Line(const std::wstring& aIn, unsigned int aUserLineNo = 0xffffffff);

                unsigned int GetUserLineNo() const;

            private:

                unsigned int mUserLineNo;

            };

            typedef std::vector<Line> Internal;

            Internal mLines;

        private:

            NO_COPY(File_UTF16);

            unsigned int RemoveLines(const std::wregex& aRegEx);

        };

    }
}
