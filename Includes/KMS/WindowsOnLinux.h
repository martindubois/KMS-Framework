
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WindowsOnLinux.h

#pragma once

// ===== C ==================================================================
#include <string.h>
#include <unistd.h>

// Data types
// //////////////////////////////////////////////////////////////////////////

typedef int SOCKET;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define INVALID_HANDLE_VALUE (-1)
#define INVALID_SOCKET       (-1)
#define SB_BOTH              SHUT_RDWR

// Macros
// //////////////////////////////////////////////////////////////////////////

#define errno_t int

#define _crt_signal_t sighandler_t

#define _stricmp  strcasecmp
#define _strnicmp strncasecmp

#define closesocket close

// ===== ..._s ==============================================================

#define sprintf_s sprintf
#define sscanf_s  sscanf
#define strcat_s  strcat
#define strcpy_s  strcpy
#define strncpy_s strncpy

#define fopen_s(F,N,M) ((NULL != (*(F) = fopen((N), (M)))) ? 0 : -1)
