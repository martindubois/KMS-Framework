
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/Component.h

#pragma once

#include <KMS/Base.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <WinSock2.h>
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Exception.h>
#include <KMS/Test/Test.h>
