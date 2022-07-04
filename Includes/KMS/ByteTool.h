
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/ByteTool.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurable.h>

namespace KMS
{

    class ByteTool : public Cfg::Configurable
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

        // ===== Cfg::Configurable ==========================================
        virtual bool SetAttribute(const char* aA, const char* aV);
        virtual bool SetAttribute_Indexed(const char* aA, const char* aI, const char * aV);
        virtual void DisplayHelp(FILE* aOut) const;
        
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
