
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Base.h

#pragma once

#ifdef _WIN32
    #define _KMS_WINDOWS_
#else
    #ifdef __APPLE__
        #define _KMS_DARWIN_
    #else
        #define _KMS_LINUX_
    #endif
#endif

// ===== C ==================================================================
#include <assert.h>

// ===== Includes ===========================================================
#include <KMS/SafeAPI.h>

#if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
    #include <KMS/WindowsOnLinux.h>
#endif

// Constants
// //////////////////////////////////////////////////////////////////////////

#define FILE_LENGTH (256)
#define LINE_LENGTH (512)
#define NAME_LENGTH (128)
#define PATH_LENGTH (512)

#ifdef _KMS_WINDOWS_
    #define OS_NAME "Windows"

    #define SLASH   "\\"
    #define SLASH_C '\\'

    #define PRINTF_64_U "%llu"
    #define PRINTF_64_X "%016llx"
#endif

#if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
    #define SLASH   "/"
    #define SLASH_C '/'

    #define PRINTF_64_U "%lu"
    #define PRINTF_64_X "%lx"
#endif

#ifdef OS_PROC
    #define OS_NAME_PROC OS_NAME "_" OS_PROC
#else
    #define OS_NAME_PROC OS_NAME
#endif

// Macros
// //////////////////////////////////////////////////////////////////////////

#define NO_COPY(C)              \
    C(const C &);               \
    void operator = (const C &)
