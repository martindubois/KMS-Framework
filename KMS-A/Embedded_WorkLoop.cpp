
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

        WorkLoop::WorkLoop() : mFirst(NULL) {}

        void WorkLoop::AddItem(WorkItem* aItem)
        {
            // assert(NULL != aItem);

            aItem->mNext = mFirst;

            mFirst = aItem;
        }

        void WorkLoop::Loop()
        {
            WorkItem* lCurrent = mFirst;

            while (NULL != lCurrent)
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
