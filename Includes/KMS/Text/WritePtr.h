
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/WritePtr.h

#pragma once

namespace KMS
{
    namespace Text
    {

        class WritePtr
        {

        public:

            WritePtr(char* aBase, unsigned int aSize_byte);

            ~WritePtr();

            operator char* ();

            void operator += (unsigned int aInc);

            void Write(char aIn);

            void Write(const char* aIn, unsigned int aInSize_byte);

            void Write(const char* aIn);

            unsigned int GetIndex() const;

            unsigned int GetRemainingSize() const;

        private:

            char       * mBase;
            unsigned int mIndex_byte;
            unsigned int mSize_byte;

        };

    }
}
