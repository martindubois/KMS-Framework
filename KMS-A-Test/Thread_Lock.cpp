
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Thread_Lock.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Thread/Gate.h>
#include <KMS/Thread/Lock.h>

using namespace KMS;

KMS_TEST(Thread_Lock_Base, "Auto", sTest_Base)
{
    Thread::Gate lG;
    Thread::Lock lL(&lG);

    lL.Unlock();
    lL.Relock();
}
