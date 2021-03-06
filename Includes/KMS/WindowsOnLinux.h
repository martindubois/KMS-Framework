
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

// ===== ..._s ==============================================================

#define _stricmp strcasecmp

#define sprintf_s sprintf
#define sscanf_s  sscanf

#define fopen_s(F,N,M) ((NULL != (*(F) = fopen((N), (M)))) ? 0 : -1)
