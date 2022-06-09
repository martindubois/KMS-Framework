
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/ByteTool.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>

namespace KMS
{

    class ByteTool : public Config::Configurable
    {

    public:

        static int Main(int aCount, const char** aVector);

        ByteTool();

        ~ByteTool();

        const unsigned int* GetHistrogram() const;

        int Run();

        void SetByteTable(unsigned int aIndex, uint8_t aValue);

        void SetDestination(FILE* aFile);
        void SetSource     (FILE* aFile);

        // ===== Config::Configurable =======================================
        virtual bool SetAttribute(const char* aA, const char* aV);
        virtual bool SetAttribute(const char* aA, unsigned int aI, const char * aV);
        
    private:

        void CloseFile(FILE * aFile);

        unsigned int mHistogram[256];

        // ===== Configurable attributes ====================================
        uint8_t mByteTable[256];
        FILE  * mDestination;
        FILE  * mSource;

    };

}

std::ostream& operator << (std::ostream& aOut, const KMS::ByteTool& aBT);
