
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Text/File.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace Text
    {

        class File
        {

        public:

            static const unsigned int FLAG_DO_NOT_REMOVE_CR = 0x00000001;

            virtual void Clear() = 0;

            virtual unsigned int GetLineCount() const = 0;

            // Exception  TEXT_ARGUMENT_INVALID
            virtual unsigned int GetUserLineNo(unsigned int aNo) const = 0;

            virtual unsigned int RemoveEmptyLines() = 0;

            // Exception  TEXT_ARGUMENT_INVALID
            virtual void RemoveLines(unsigned int aNo, unsigned int aCount) = 0;

            // aFlags  FLAG_DO_NOT_REMOVE_CR
            // Exception  TEXT_OPEN_FAILED
            virtual void Read(const KMS::File::Folder& aFolder, const char* aFileName, unsigned int aFlags = 0) = 0;

            virtual unsigned int RemoveComments_CPP   () = 0;
            virtual unsigned int RemoveComments_Script() = 0;

        };

    }
}
