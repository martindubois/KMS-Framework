
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/ReadPtr.h

#pragma once

namespace KMS
{
    namespace Text
    {

        class ReadPtr final
        {

        public:

            ReadPtr(const char* aBase, unsigned int aSize_byte);

            operator const char * () const;

            void operator += (unsigned int aA);

            unsigned int GetIndex() const;

            bool IsAtEnd() const;

            void After(char aC);

            void After(const char* aStr);

            unsigned int ExtractUntil(char aC, char* aOut, unsigned int aOutSize_byte);

            unsigned int ExtractUntil(const char* aStr, char* aOut, unsigned int aOutSize_byte);

            unsigned int ExtractWhile(const char* aStr, char* aOut, unsigned int aOutSize_byte);

            void Next(char aC);

            void Next(const char* aStr);

            void Skip(const char* aStr);

            void SkipBlank();

            void Test(char aC);

            void Test(const char* aStr);

        private:

            const char * mBase;
            unsigned int mIndex_byte;
            unsigned int mSize_byte;

        };

    }
}
