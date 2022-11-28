
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/KMS-STM32/Needed.cpp

#include "Component.h"

// Entry points
// //////////////////////////////////////////////////////////////////////////

extern "C"
{

    void exit(int) { for (;;) {} }

    void _exit(int) { for (;;) {} }

    int _getpid() {	return 1; }

    int _kill(int, int) { return -1; }

    void* _sbrk(ptrdiff_t) { return reinterpret_cast<void*>(-1); }

}
