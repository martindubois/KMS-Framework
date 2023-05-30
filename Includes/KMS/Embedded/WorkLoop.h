
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/WorkLoop.h

#pragma once

namespace KMS
{
    namespace Embedded
    {

        class WorkItem;

        class WorkLoop
        {

        public:

            WorkLoop();

            void AddItem(WorkItem* aItem);

            void Loop();

            void Run();

        private:

            NO_COPY(WorkLoop);

            WorkItem* mFirst;

        };

    }
}
