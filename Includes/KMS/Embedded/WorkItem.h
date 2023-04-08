
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/WorkItem.h

#pragma once

namespace KMS
{
    namespace Embedded
    {

        class WorkItem
        {

        public:

            virtual void Work() = 0;

            WorkItem* mNext;

        protected:

            WorkItem();

        private:

            NO_COPY(WorkItem);

        };

    }
}
