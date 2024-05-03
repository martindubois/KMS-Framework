
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/File_UTF16.h
// Status    PROD_READY (Windows)
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <regex>
#include <vector>

// ===== Includes ===========================================================
#include <KMS/Text/File.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Text
    {

        class File_UTF16 final : public File
        {

        public:

            File_UTF16();

            void AddLine(const wchar_t* aLine);

            // Exception  TEXT_ARGUMENT_INVALID
            const wchar_t* GetLine(unsigned int aNo) const;

            // Exception  TEXT_ARGUMENT_INVALID
            void InsertLine(unsigned int aNo, const wchar_t* aLine);

            // Exception  TEXT_ARGUMENT_INVALID
            void ReplaceLine(unsigned int aNo, const wchar_t* aLine);

            unsigned int ReplaceLines(const wchar_t* aRegEx, const wchar_t* aReplace);

            // Exception  TEXT_OPEN_FAILED
            void Write(const KMS::File::Folder& aFolder, const char* aFileName, const wchar_t* aEOL = L"\n");

            // Exception  TEXT_OPEN_FAILED
            void Write_ASCII(const KMS::File::Folder& aFolder, const char* aFileName, const char* aEOL = "\n");

            unsigned int CountOccurrence(const wchar_t* aStr) const;

            // ===== File ===================================================
            virtual void Clear();
            virtual unsigned int GetLineCount() const;
            virtual unsigned int GetUserLineNo(unsigned int aNo) const;
            virtual unsigned int RemoveEmptyLines();
            virtual void RemoveLines(unsigned int aNo, unsigned int aCount);
            virtual void Read(const KMS::File::Folder& aFolder, const char* aFileName, unsigned int aFlags = 0);
            virtual unsigned int RemoveComments_CPP   ();
            virtual unsigned int RemoveComments_Script();

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

            // Exception  TEXT_ARGUMENT_INVALID
            const Line& GetLine2(unsigned int aNo) const;

            Internal mLines;

        private:

            NO_COPY(File_UTF16);

            unsigned int RemoveLines(const std::wregex& aRegEx);

        };

    }
}
