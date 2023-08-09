
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Embedded_WorkLoop.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/WorkItem.h>

#include <KMS/Embedded/WorkLoop.h>

namespace KMS
{
    namespace Embedded
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        WorkLoop::WorkLoop() : mFirst(nullptr) {}

        void WorkLoop::AddItem(WorkItem* aItem)
        {
            // assert(nullptr != aItem);

            aItem->mNext = mFirst;

            mFirst = aItem;
        }

        void WorkLoop::Loop()
        {
            WorkItem* lCurrent = mFirst;

            while (nullptr != lCurrent)
            {
                lCurrent->Work();

                lCurrent = lCurrent->mNext;
            }
        }

        void WorkLoop::Run()
        {
            for (;;)
            {
                Loop();
            }
        }

    }
}
