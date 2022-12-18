
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/ByteTool.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/File.h>

namespace KMS
{

    class ByteTool : public DI::Dictionary
    {

    public:

        static int Main(int aCount, const char** aVector);

        ByteTool();

        ~ByteTool();

        const unsigned int* GetHistrogram() const;

        int Run();

        // ===== Configurable attributes ====================================
        DI::File mSource;

    // Internal

        void Display(std::ostream& aOut) const;
        
    private:

        unsigned int mHistogram[256];

    };

}

std::ostream& operator << (std::ostream& aOut, const KMS::ByteTool& aBT);
