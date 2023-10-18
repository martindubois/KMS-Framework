
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Component.h

#pragma once

#include <KMS/Base.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #define SECURITY_WIN32
    #include <WinSock2.h>
    #include <Windows.h>
    #include <security.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Exception.h>
