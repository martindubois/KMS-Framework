
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Base.h

#pragma once

// ===== C ==================================================================
#include <string.h>

// Macros
// //////////////////////////////////////////////////////////////////////////

#define errno_t int

#define _stricmp strcasecmp

// ===== ..._s ==============================================================

#define sprintf_s sprintf
#define sscanf_s  sscanf
#define strcpy_s  strcpy

#define fopen_s(F,N,M) ((NULL != (*(F) = fopen((N), (M)))) ? 0 : -1)
