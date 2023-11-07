
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/File_Binary.cpp

// TEST COVERAGE 2023-08-09 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

#include <KMS/File/Binary.h>

KMS_RESULT_STATIC(RESULT_ACCESS_FAILED);
KMS_RESULT_STATIC(RESULT_MAPPING_FAILED);
KMS_RESULT_STATIC(RESULT_READ_FAILED);
KMS_RESULT_STATIC(RESULT_TOO_SHORT);

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Binary::Binary(const Folder& aFolder, const char* aFile, bool aWrite)
            : mMappedSize_byte(0), mView(nullptr), mWrite(aWrite)
        {
            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            Construct_OSDep(lPath);
        }

        Binary::~Binary() { Close(); }

        unsigned int Binary::GetMappedSize() const { return mMappedSize_byte; }

    }
}
