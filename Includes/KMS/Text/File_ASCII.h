
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/File_ASCII.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <regex>

// ===== Includes ===========================================================
#include <KMS/Text/File.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Text
    {

        class File_ASCII final : public File
        {

        public:

            File_ASCII();

            void AddLine(const char* aLine);

            // Exception  TEXT_ARGUMENT_INVALID
            const char* GetLine(unsigned int aNo) const;

            // Exception  TEXT_ARGUMENT_INVALID
            void InsertLine(unsigned int aNo, const char* aLine);

            // Exception  TEXT_ARGUMENT_INVALID
            void ReplaceLine(unsigned int aNo, const char* aLine);

            unsigned int ReplaceLines(const char* aRegEx, const char* aReplace);

            // Exception  TEXT_OPEN_FAILED
            void Write(const KMS::File::Folder& aFolder, const char* aFileName, const char* aEOL = "\n");

            unsigned int CountOccurrence(const char* aStr) const;

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

            class Line : public std::string
            {

            public:

                Line(const char       * aIn, unsigned int aUserLineNo = 0xffffffff);
                Line(const std::string& aIn, unsigned int aUserLineNo = 0xffffffff);

                unsigned int GetUserLineNo() const;

            private:

                unsigned int mUserLineNo;

            };

            typedef std::vector<Line> Internal;

            // Exception  TEXT_ARGUMENT_INVALID
            const Line& GetLine2(unsigned int aNo) const;

            Internal mLines;

        private:

            NO_COPY(File_ASCII);

            unsigned int RemoveLines(const std::regex& aRegEx);

        };

    }
}
