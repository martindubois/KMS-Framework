
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/RefCount.h

#pragma once

namespace KMS
{

    class RefCount
    {

    public:

        void DecRefCount();
        void IncRefCount();

    protected:

        RefCount();

        virtual ~RefCount();

    private:

        unsigned int mRefCount;

    };

}
