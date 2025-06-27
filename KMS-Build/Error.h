
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Error.h

#pragma once

// Functions
// //////////////////////////////////////////////////////////////////////////

extern void Error_File_AlreadyExist(const char* aName);
extern void Error_File_DoesNotExist(const char* aName);

extern void Error_Folder_DoesNotExist(const char* aName);
