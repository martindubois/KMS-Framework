
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/OS.h

#pragma once

namespace KMS
{
    namespace OS
    {

        uint32_t GetProcessId();

        uint64_t GetSystemTime();

        uint32_t GetThreadId();

    }
}
