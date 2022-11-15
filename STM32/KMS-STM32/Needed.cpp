
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      STM32/F373CVTx/KMS-STM32/Needed.cpp

#include "Component.h"

// Entry points
// //////////////////////////////////////////////////////////////////////////

extern "C"
{

    void exit(int)
    {
        for (;;) {}
    }

}
