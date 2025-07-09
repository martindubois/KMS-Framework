
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Error.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Error.h"

KMS_RESULT_STATIC(RESULT_FILE_ALREADY_EXIST);
KMS_RESULT_STATIC(RESULT_FILE_DOES_NOT_EXIST);
KMS_RESULT_STATIC(RESULT_FOLDER_DOES_NOT_EXIST);

// Functions
// //////////////////////////////////////////////////////////////////////////

void Error_File_AlreadyExist(const char* aName)
{
    assert(nullptr != aName);

    char lMsg[LINE_LENGTH];

    sprintf_s(lMsg, "%s already exist", aName);

    KMS_EXCEPTION(RESULT_FILE_ALREADY_EXIST, lMsg, "");

}

void Error_File_DoesNotExist(const char* aName)
{
    assert(nullptr != aName);

    char lMsg[LINE_LENGTH];

    sprintf_s(lMsg, "%s does not exist (file)", aName);

    KMS_EXCEPTION(RESULT_FILE_DOES_NOT_EXIST, lMsg, "");
}

void Error_Folder_DoesNotExist(const char* aName)
{
    assert(nullptr != aName);

    char lMsg[LINE_LENGTH];

    sprintf_s(lMsg, "%s does not exist (folder)", aName);

    KMS_EXCEPTION(RESULT_FOLDER_DOES_NOT_EXIST, lMsg, "");

}
